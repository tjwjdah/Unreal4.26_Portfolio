// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkManager.h"
#include "DrawDebugHelpers.h"
#include "../Buildable/Buildable.h"
#include "../Buildable/DestructibleBuildable.h"
#include "../Environments/Environments.h"
#include "Components/SceneComponent.h"
#include "ChunkSaveGame.h"
#include "../Tools/ItemBox.h"
#include "../PPGameInstance.h"
#include "../AssetManager/BuildAssetSubsystem.h"
#include "../Character/Player/PlayerCharacter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "DrawDebugHelpers.h"
// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable>	TableAsset(TEXT("DataTable'/Game/Foliage/Environments/NewDataTable.NewDataTable'"));
	static ConstructorHelpers::FClassFinder<AItemBox>	Box(TEXT("Blueprint'/Game/Item/DropItem/BP_Box.BP_Box_C'"));
	if (Box.Succeeded())
		ItemBox = Box.Class;
	if (TableAsset.Succeeded())
		ResourceTableInfo = TableAsset.Object;
	
	ChunkActorClass = AChunkActor::StaticClass();
	
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();
	if (bEnableChunkStreaming)
	{
		StartChunkStreaming();
	}
	
}

int32 AChunkManager::AddResourceInstanceDefinitionName(FString ResourceDefinitionName, const FTransform& Transform, int32 ID, int32 Hp)
{
	if(!ResourceTableInfo)
		return INDEX_NONE;
	const FResourceTableInfo* Info = ResourceTableInfo->FindRow<FResourceTableInfo>(*ResourceDefinitionName, "");
	
	if(!Info)
		return INDEX_NONE;

	UStaticMesh* StaticMesh = Info->StaticMesh.LoadSynchronous();
	
	if(!StaticMesh)
		return INDEX_NONE;
	int32 NewID = (ID == INDEX_NONE) ? ResourceID : ID;
	int32 NewHp = (Hp == INDEX_NONE) ? Info->Hp : Hp;



	AChunkActor* Target = GetChunk(Transform.GetLocation());
	
	if (!Target)
		return INDEX_NONE;

	int32 NewIndex = -1;
	
	if (Target) {
		NewIndex = Target->AddInstance(StaticMesh, Transform, NewID);
	}

	if (NewIndex == -1)
		return INDEX_NONE;
	FResourceNode NewNode(Target, NewIndex, StaticMesh, NewHp, Info->Type, ResourceDefinitionName);
	ResourceMap.Add(NewID, NewNode);

	if (ID == INDEX_NONE)
	{
		ResourceID++;
	}
	else
	{
		ResourceID = FMath::Max(ResourceID, ID + 1);
	}

	MarkChunkDirtyLocation(Transform.GetLocation());
	return NewID;
}

void AChunkManager::StartChunkStreaming()
{
	bEnableChunkStreaming = true;
	bStreamingStart = false;
	bHasLastStreamingCenter = false;
	StreamingUpdateElapse = 0.f;

	ClearAll();

	if (!LoadRuntimeSaveCache())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] Failed to load runtime save cache."));
		return;
	}

	DirtyChunks.Empty();
	GraphDirtyChunks.Empty();
	bStreamingStart = true;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
		return;
	UpdateChunkStreamingAroundLocation(PlayerPawn->GetActorLocation(), false);

	RecalculateStreamingStats();
	LogChunkStreamingStats(TEXT("StartChunkStreaming"));

}

void AChunkManager::StopChunkStreaming()
{
	if (!bStreamingStart)
		return;

	FlushDirtyChunksSaveCache();
	SaveRuntimeSaveCacheSlot();

	bStreamingStart = false;
	bEnableChunkStreaming = false;

	RecalculateStreamingStats();
	LogChunkStreamingStats(TEXT("StopChunkStreaming"));
}


void AChunkManager::ForceUpdateChunkStreaming()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
		return;
	
	UpdateChunkStreamingAroundLocation(PlayerPawn->GetActorLocation(), true);
}

void AChunkManager::PrintChunkStreamingStats_Editor()
{
	LogChunkStreamingStats(TEXT("EditorPrint"));
}

AChunkActor* AChunkManager::GetChunkCoords(const FChunkCoords& Coords)
{

	if (AChunkActor* LoadedChunk = FindLoadChunk(Coords))
	{
		return LoadedChunk;
	}

	return SpawnChunkActorOnly(Coords);
}



void AChunkManager::UpdateChunkStreamingAroundLocation(const FVector& Location, bool bForce)
{
	if (!bStreamingStart)
		return;

	FChunkCoords CenterCoords = GetChunkCoordsLocation(Location);

	if (!bForce && bHasLastStreamingCenter && LastStreamingCenter == CenterCoords && GraphDirtyChunks.Num() == 0)
	{
		RecalculateStreamingStats();
		return;
	}

	LastStreamingCenter = CenterCoords;
	bHasLastStreamingCenter = true;

	for (const FChunkCoords& Coords : GraphDirtyChunks)
	{
		if (LoadChunks.Contains(Coords))
		{
			SaveLoadChunkCache(Coords);
		}
	}
	GraphDirtyChunks.Empty();
	TSet<FChunkCoords> WantChunks = BuildWantChunkSet(CenterCoords);
	TSet<FChunkCoords> FinalWantChunks = BuildConnectedWantedChunkSet(WantChunks);
	//TSet<FChunkCoords> FinalWantChunks = WantChunks;
	TArray<FChunkCoords> CurrentLoadCoords;
	LoadChunks.GetKeys(CurrentLoadCoords);

	TArray<FChunkCoords> Unload;
	TArray<FChunkCoords> Load;


	for (const FChunkCoords& LoadCoords : CurrentLoadCoords)
	{
		if (!FinalWantChunks.Contains(LoadCoords))
		{
			Unload.Add(LoadCoords);
		}
	}

	for (const FChunkCoords& WantCoords : FinalWantChunks)
	{
		if (!LoadChunks.Contains(WantCoords))
		{
			Load.Add(WantCoords);
		}
	}

	if (ChunkDataIndexCoords.Contains(CenterCoords)) {
		bool bLoadFail = LoadChunkRuntime(CenterCoords);
		if (!bLoadFail)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] Failed to load center chunk %s"), *ChunkCoordsString(CenterCoords));
			RecalculateStreamingStats();
			return;
		}
	}

	for (const FChunkCoords& Coords : Unload) {
		UnloadChunkRuntime(Coords);
	}

	
	for (const FChunkCoords& Coords : Load)
	{
		if (Coords == CenterCoords)
			continue;
		if (ChunkDataIndexCoords.Contains(Coords)) {
			LoadChunkRuntime(Coords);
		}
	}
	

	RecalculateStreamingStats();

	if (bLogChunkStreaming)
		LogChunkStreamingStats(TEXT("UpdateStreaming"));

	if (bWriteStreamingCSV)
		AppendChunkStreamingCSV(TEXT("UpdateStreaming"));
}

bool AChunkManager::LoadRuntimeSaveCache()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex)) {
		RuntimeSaveGame = Cast<UChunkSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	}
	else {
		RuntimeSaveGame = Cast<UChunkSaveGame>(UGameplayStatics::CreateSaveGameObject(UChunkSaveGame::StaticClass()));

		if (RuntimeSaveGame) {
			RuntimeSaveGame->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
			RuntimeSaveGame->SaveVersion = 1;
			RuntimeSaveGame->NextBuildingID = BuildingID;
			RuntimeSaveGame->NextResourceID = ResourceID;
		}
	}

	if(!RuntimeSaveGame)
		return false;

	BuildingID = FMath::Max(BuildingID, RuntimeSaveGame->NextBuildingID);
	ResourceID = FMath::Max(ResourceID, RuntimeSaveGame->NextResourceID);
	RebuildChunkDataIndex();

	return true;
}

void AChunkManager::RebuildChunkDataIndex()
{
	ChunkDataIndexCoords.Empty();
	ChunkBuildingDataIndexCoords.Empty();
	if (!RuntimeSaveGame)
		return;

	for (int32 i = 0; i < RuntimeSaveGame->Chunks.Num(); i++) {
		ChunkDataIndexCoords.Add(RuntimeSaveGame->Chunks[i].Coords, i);
		for (const FBuildingSaveData& BuildingData : RuntimeSaveGame->Chunks[i].Buildings)
		{
			ChunkBuildingDataIndexCoords.Add(BuildingData.ID, RuntimeSaveGame->Chunks[i].Coords);
		}
	}
}

bool AChunkManager::SaveRuntimeSaveCacheSlot()
{
	if (!RuntimeSaveGame)
		return false;

	RuntimeSaveGame->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	RuntimeSaveGame->SaveVersion = 1;
	RuntimeSaveGame->NextBuildingID = BuildingID;
	RuntimeSaveGame->NextResourceID = ResourceID;
	
	bool bSaved = UGameplayStatics::SaveGameToSlot(RuntimeSaveGame,SaveSlotName,SaveUserIndex);
	
	if (bSaved)
	{
		DirtyChunks.Empty();
		GraphDirtyChunks.Empty();
	}

	return bSaved;
}

void AChunkManager::RemoveSaveCacheChunkData(const FChunkCoords& Coords)
{
	if (!RuntimeSaveGame)
	{
		return;
	}

	const int32* FoundIndex = ChunkDataIndexCoords.Find(Coords);

	if (!FoundIndex)
	{
		return;
	}

	if (RuntimeSaveGame->Chunks.IsValidIndex(*FoundIndex))
	{
		RuntimeSaveGame->Chunks.RemoveAt(*FoundIndex);
	}

	RebuildChunkDataIndex();
}

void AChunkManager::RecalculateStreamingStats()
{
	TSet<FChunkCoords> KnownCoords;

	if (RuntimeSaveGame)
	{
		for (const FChunkSaveData& ChunkData : RuntimeSaveGame->Chunks)
		{
			KnownCoords.Add(ChunkData.Coords);
		}
	}

	for (const auto& Pair : LoadChunks)
	{
		KnownCoords.Add(Pair.Key);
	}
	ChunkStreamingStats.TotalKnownChunks = KnownCoords.Num();
	ChunkStreamingStats.ActiveChunks = LoadChunks.Num();
	ChunkStreamingStats.UnloadChunks =FMath::Max(0, ChunkStreamingStats.TotalKnownChunks - ChunkStreamingStats.ActiveChunks);
	ChunkStreamingStats.DirtyChunkCount = DirtyChunks.Num();

	ChunkStreamingStats.ActiveBuildingInstances = 0;
	ChunkStreamingStats.ActiveResourceInstances = 0;
	ChunkStreamingStats.ActiveTotalInstances = 0;
	ChunkStreamingStats.ActiveHISMComponents = 0;

	for (const auto& Pair : LoadChunks)
	{
		AChunkActor* Chunk = Pair.Value;

		if (!IsValid(Chunk))
		{
			continue;
		}

		ChunkStreamingStats.ActiveBuildingInstances += Chunk->GetBuildingInstanceCount();
		ChunkStreamingStats.ActiveResourceInstances += Chunk->GetResourceInstanceCount();
		ChunkStreamingStats.ActiveHISMComponents += Chunk->GetHISMComponentCount();
	}

	ChunkStreamingStats.ActiveTotalInstances = ChunkStreamingStats.ActiveBuildingInstances + ChunkStreamingStats.ActiveResourceInstances;

	ChunkStreamingStats.AverageChunkLoadTime = ChunkStreamingStats.ChunkLoadCount > 0 ? ChunkStreamingStats.TotalChunkLoadTime / ChunkStreamingStats.ChunkLoadCount : 0.f;

	ChunkStreamingStats.AverageChunkUnloadTime = ChunkStreamingStats.ChunkUnloadCount > 0 ? ChunkStreamingStats.TotalChunkUnloadTime / ChunkStreamingStats.ChunkUnloadCount : 0.f;

	ChunkStreamingStats.AverageChunkSaveTime = ChunkStreamingStats.ChunkSaveCount > 0 ? ChunkStreamingStats.TotalChunkSaveTime / ChunkStreamingStats.ChunkSaveCount: 0.f;
}

void AChunkManager::LogChunkStreamingStats(const FString& String)
{
	RecalculateStreamingStats();

	UE_LOG(LogTemp, Log, TEXT("========== [ChunkStreaming] %s =========="), *String);
	UE_LOG(LogTemp, Log, TEXT("TotalKnownChunks       : %d"), ChunkStreamingStats.TotalKnownChunks);
	UE_LOG(LogTemp, Log, TEXT("ActiveChunks           : %d"), ChunkStreamingStats.ActiveChunks);
	UE_LOG(LogTemp, Log, TEXT("UnloadedChunks         : %d"), ChunkStreamingStats.UnloadChunks);
	UE_LOG(LogTemp, Log, TEXT("DirtyChunks            : %d"), ChunkStreamingStats.DirtyChunkCount);
	UE_LOG(LogTemp, Log, TEXT("ActiveBuildingInstances: %d"), ChunkStreamingStats.ActiveBuildingInstances);
	UE_LOG(LogTemp, Log, TEXT("ActiveResourceInstances: %d"), ChunkStreamingStats.ActiveResourceInstances);
	UE_LOG(LogTemp, Log, TEXT("ActiveTotalInstances   : %d"), ChunkStreamingStats.ActiveTotalInstances);
	UE_LOG(LogTemp, Log, TEXT("ActiveHISMComponents   : %d"), ChunkStreamingStats.ActiveHISMComponents);
	UE_LOG(LogTemp, Log, TEXT("AvgChunkLoadTimeMs     : %.3f"), ChunkStreamingStats.AverageChunkLoadTime);
	UE_LOG(LogTemp, Log, TEXT("AvgChunkUnloadTimeMs   : %.3f"), ChunkStreamingStats.AverageChunkUnloadTime);
	UE_LOG(LogTemp, Log, TEXT("AvgChunkSaveTimeMs     : %.3f"), ChunkStreamingStats.AverageChunkSaveTime);
}


void AChunkManager::AppendChunkStreamingCSV(const FString& String)
{
	RecalculateStreamingStats();

	const FString FilePath = FPaths::ProjectSavedDir() / StreamingCSVFileName;

	if (!FPaths::FileExists(FilePath))
	{
		const FString Header =
			TEXT("Reason,TotalKnownChunks,ActiveChunks,UnloadedChunks,DirtyChunks,")
			TEXT("ActiveBuildingInstances,ActiveResourceInstances,ActiveTotalInstances,ActiveHISMComponents,")
			TEXT("ChunkLoadCount,ChunkUnloadCount,ChunkSaveCount,")
			TEXT("AvgLoadMs,AvgUnloadMs,AvgSaveMs\n");

		FFileHelper::SaveStringToFile(Header, *FilePath);
	}

	const FString Line = FString::Printf(
		TEXT("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.3f,%.3f,%.3f\n"),
		*String,
		ChunkStreamingStats.TotalKnownChunks,
		ChunkStreamingStats.ActiveChunks,
		ChunkStreamingStats.UnloadChunks,
		ChunkStreamingStats.DirtyChunkCount,
		ChunkStreamingStats.ActiveBuildingInstances,
		ChunkStreamingStats.ActiveResourceInstances,
		ChunkStreamingStats.ActiveTotalInstances,
		ChunkStreamingStats.ActiveHISMComponents,
		ChunkStreamingStats.ChunkLoadCount,
		ChunkStreamingStats.ChunkUnloadCount,
		ChunkStreamingStats.ChunkSaveCount,
		ChunkStreamingStats.AverageChunkLoadTime,
		ChunkStreamingStats.AverageChunkUnloadTime,
		ChunkStreamingStats.AverageChunkSaveTime
	);

	FFileHelper::SaveStringToFile(Line,*FilePath,FFileHelper::EEncodingOptions::AutoDetect,&IFileManager::Get(),FILEWRITE_Append);
}

bool AChunkManager::LoadChunkRuntime(const FChunkCoords& Coords)
{
	if (LoadChunks.Contains(Coords))
		return true;

	double StartSeconds = FPlatformTime::Seconds();

	AChunkActor* Chunk = GetChunkCoords(Coords);

	if (!Chunk)
		return false;

	FChunkSaveData* ChunkSaveData = FindSaveCacheChunkData(Coords);

	if (ChunkSaveData) {
		bool PrevSuppressDirty = SuppressDirtyTracking;
		SuppressDirtyTracking = true;

		for (const FResourceSaveData& ResourceData : ChunkSaveData->Resources) {
			AddResourceInstanceDefinitionName(ResourceData.ResourceDefinitionName, ResourceData.Transform, ResourceData.ID, ResourceData.Hp);
		}

		for (const FBuildingSaveData& BuildingData : ChunkSaveData->Buildings)
			AddBuildingInstanceDefinitionName(BuildingData.BuildDefinitionName, BuildingData.Transform, BuildingData.ID, BuildingData.Durability);

		for (const FBuildingSaveData& BuildingData : ChunkSaveData->Buildings)
		{
			FBuildingNode* Node = BuildingMap.Find(BuildingData.ID);
			if (!Node)
				continue;

			Node->SocketMap = BuildingData.SocketMap;
			Node->ConnectedBuildingIDs.Empty();

			for (const auto& SocketPair : Node->SocketMap)
			{
				if (SocketPair.Value >= 0)
					Node->ConnectedBuildingIDs.AddUnique(SocketPair.Value);
			}

		}

		for (const FAttachBuildableSaveData& AttachSaveData : ChunkSaveData->AttachBuildables)
			SpawnAttachBuildableSaveData(AttachSaveData);

		SuppressDirtyTracking = PrevSuppressDirty;
	}

	float Elapse = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

	ChunkStreamingStats.ChunkLoadCount++;
	ChunkStreamingStats.TotalChunkLoadTime += Elapse;
	ChunkStreamingStats.AverageChunkLoadTime = ChunkStreamingStats.ChunkLoadCount > 0 ? ChunkStreamingStats.TotalChunkLoadTime / ChunkStreamingStats.ChunkLoadCount : 0.f;

	if (bLogChunkStreaming)
		UE_LOG(LogTemp, Log, TEXT("[ChunkStreaming] Load Chunk %s / %.3f ms"), *ChunkCoordsString(Coords), Elapse);
	return true;
}

bool AChunkManager::UnloadChunkRuntime(const FChunkCoords& Coords)
{
	AChunkActor** FoundChunk = LoadChunks.Find(Coords);

	if (!FoundChunk || !IsValid(*FoundChunk)) {
		LoadChunks.Remove(Coords);
		return false;
	}
	double StartSeconds = FPlatformTime::Seconds();

	AChunkActor* Chunk = *FoundChunk;

	if (DirtyChunks.Contains(Coords) || !FindSaveCacheChunkData(Coords))
		SaveLoadChunkCache(Coords);

	TSet<int32> RemoveBuildingIDs;
	TSet<int32> RemoveResourceIDs;

	for (const auto& Pair : BuildingMap) {
		const FBuildingNode& Node = Pair.Value;
		if (Node.OwnerChunk == Chunk) {
			RemoveBuildingIDs.Add(Pair.Key);

		}
	}

	for (const auto& Pair : ResourceMap)
	{
		const FResourceNode& Node = Pair.Value;

		if (Node.OwnerChunk == Chunk)
		{
			RemoveResourceIDs.Add(Pair.Key);
		}

	}

	TArray<FAttachBuildKey> RemoveAttachKeys;

	for (const auto& Pair : HISMAttachBuildActors) {
		if (RemoveBuildingIDs.Contains(Pair.Key.ParentBuildingID))
			RemoveAttachKeys.Add(Pair.Key);
	}

	for (const FAttachBuildKey& Key : RemoveAttachKeys) {
		ABuildable** FoundBuildable = HISMAttachBuildActors.Find(Key);

		if (FoundBuildable && IsValid(*FoundBuildable))
			(*FoundBuildable)->Destroy();
		HISMAttachBuildActors.Remove(Key);
	}

	for (int32 RemoveID : RemoveBuildingIDs)
		BuildingMap.Remove(RemoveID);

	for (int32 RemoveID : RemoveResourceIDs)
		ResourceMap.Remove(RemoveID);

	Chunk->ClearAll();
	Chunk->Destroy();

	LoadChunks.Remove(Coords);


	float Elapse = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

	ChunkStreamingStats.ChunkUnloadCount++;
	ChunkStreamingStats.TotalChunkUnloadTime += Elapse;
	ChunkStreamingStats.AverageChunkUnloadTime = ChunkStreamingStats.ChunkUnloadCount > 0 ? ChunkStreamingStats.TotalChunkUnloadTime
		/ ChunkStreamingStats.ChunkUnloadCount : 0.f;

	if (bLogChunkStreaming)
	{
		UE_LOG(LogTemp, Log, TEXT("[ChunkStreaming] Unload Chunk %s / %.3f ms / Buildings:%d Resources:%d"), *ChunkCoordsString(Coords),
			Elapse, RemoveBuildingIDs.Num(), RemoveResourceIDs.Num());
	}

	return true;
}
/*
bool AChunkManager::LoadChunkRuntime(const FChunkCoords& Coords, TSet<int32>& LoadChunkBuildingIDs, TSet<FChunkCoords>& ConnectLoadCoords)
{
	if (LoadChunks.Contains(Coords)) {

		return true;
	}
	double StartSeconds = FPlatformTime::Seconds();

	AChunkActor* Chunk = GetChunkCoords(Coords, true);

	if (!Chunk)
		return false;

	FChunkSaveData* ChunkSaveData = FindSaveCacheChunkData(Coords);

	if (ChunkSaveData) {
		bool PrevSuppressDirty = SuppressDirtyTracking;
		SuppressDirtyTracking = true;

		for (const FResourceSaveData& ResourceData : ChunkSaveData->Resources) {
			AddResourceInstanceDefinitionName(ResourceData.ResourceDefinitionName,ResourceData.Transform, ResourceData.ID, ResourceData.Hp);
		}

		for (const FBuildingSaveData& BuildingData : ChunkSaveData->Buildings)
			AddBuildingInstanceDefinitionName(BuildingData.BuildDefinitionName, BuildingData.Transform,BuildingData.ID,BuildingData.Durability);

		for (const FBuildingSaveData& BuildingData : ChunkSaveData->Buildings)
		{
			FBuildingNode* Node = BuildingMap.Find(BuildingData.ID);
			if (!Node)
				continue;

			Node->SocketMap = BuildingData.SocketMap;
			Node->ConnectedBuildingIDs.Empty();
			
			TSet<int32> SetIDs;
			for (const auto& SocketPair : Node->SocketMap)
			{
				if (SocketPair.Value >= 0) {
					Node->ConnectedBuildingIDs.AddUnique(SocketPair.Value);
					SetIDs.Add(SocketPair.Value);
				}
			}
			
			for (int32 ID : SetIDs)
			{
				const FChunkCoords* FoundCoords = ChunkBuildingDataIndexCoords.Find(ID);

				if (!FoundCoords)
					continue;

				if(ConnectLoadCoords.Contains(*FoundCoords) || LoadChunkBuildingIDs.Contains(ID))
					continue;
				TQueue<int32> Q;
				Q.Enqueue(ID);

				while (!Q.IsEmpty())
				{
					int32 CurrentID;
					Q.Dequeue(CurrentID);
					LoadChunkBuildingIDs.Add(CurrentID);
					const FChunkCoords* FoundCoords2 = ChunkBuildingDataIndexCoords.Find(CurrentID);

					if (!FoundCoords2)
						continue;
					FChunkSaveData* ChunkSaveData2 = FindSaveCacheChunkData(*FoundCoords2);
					if (!ChunkSaveData2)
						continue;
					for (const FBuildingSaveData& Building : ChunkSaveData2->Buildings) {
						for (const auto& Pair : Building.SocketMap)
						{
							const FChunkCoords* FoundCoords3 = ChunkBuildingDataIndexCoords.Find(Pair.Value);

							if (!FoundCoords3)
								continue;

							if (!ConnectLoadCoords.Contains(*FoundCoords3) && !LoadChunkBuildingIDs.Contains(Pair.Value))
							{
								Q.Enqueue(Pair.Value);
								LoadChunkBuildingIDs.Add(Pair.Value);
								ConnectLoadCoords.Add(*FoundCoords3);
							}
						}
					}
				}
			}
			
		}

		for (const FAttachBuildableSaveData& AttachSaveData : ChunkSaveData->AttachBuildables)
			SpawnAttachBuildableSaveData(AttachSaveData);

		SuppressDirtyTracking = PrevSuppressDirty;
	}

	float Elapse = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

	ChunkStreamingStats.ChunkLoadCount++;
	ChunkStreamingStats.TotalChunkLoadTime += Elapse;
	ChunkStreamingStats.AverageChunkLoadTime = ChunkStreamingStats.ChunkLoadCount > 0 ? ChunkStreamingStats.TotalChunkLoadTime / ChunkStreamingStats.ChunkLoadCount : 0.f;

	if (bLogChunkStreaming)
		UE_LOG(LogTemp,Log,TEXT("[ChunkStreaming] Load Chunk %s / %.3f ms"), *ChunkCoordsString(Coords), Elapse);
	return true;
}
/*
bool AChunkManager::UnloadChunkRuntime(const FChunkCoords& Coords, TSet<int32>& UnLoadChunkBuildingIDs, TSet<FChunkCoords>& ConnectUnLoadCoords)
{
	AChunkActor** FoundChunk = LoadChunks.Find(Coords);

	if (!FoundChunk || !IsValid(*FoundChunk)) {
		LoadChunks.Remove(Coords);
		return false;
	}
	double StartSeconds = FPlatformTime::Seconds();

	AChunkActor* Chunk = *FoundChunk;

	if (DirtyChunks.Contains(Coords) || !FindSaveCacheChunkData(Coords))
		SaveLoadChunkCache(Coords);

	TSet<int32> RemoveBuildingIDs;
	TSet<int32> RemoveResourceIDs;

	for (const auto& Pair : BuildingMap) {
		const FBuildingNode& Node = Pair.Value;
		if (Node.OwnerChunk == Chunk) {
			RemoveBuildingIDs.Add(Pair.Key);

			TSet<int32> SetIDs;
			for (const auto& SocketPair : Node.SocketMap)
			{
				if (SocketPair.Value >= 0)
					SetIDs.Add(SocketPair.Value);
			}

			for (int32 ID : SetIDs)
			{
				const FChunkCoords* FoundCoords = ChunkBuildingDataIndexCoords.Find(ID);

				if (!FoundCoords)
					continue;
				if (ConnectUnLoadCoords.Contains(*FoundCoords) || UnLoadChunkBuildingIDs.Contains(ID))
					continue;
				TQueue<int32> Q;
				Q.Enqueue(ID);
				while (!Q.IsEmpty())
				{
					
					int32 CurrentID;
					Q.Dequeue(CurrentID);
					UnLoadChunkBuildingIDs.Add(CurrentID);
					FBuildingNode* Node2 = BuildingMap.Find(CurrentID);
					if (!Node2)
						continue;
					for (const auto& Pair2 : Node2->SocketMap) {
						const FChunkCoords* FoundCoords2 = ChunkBuildingDataIndexCoords.Find(Pair2.Value);

						if (!FoundCoords2)
							continue;
						if (BuildingMap.Find(Pair2.Value) && !ConnectUnLoadCoords.Contains(*FoundCoords2) && !UnLoadChunkBuildingIDs.Contains(ID))
						{
							Q.Enqueue(Pair2.Value);
							UnLoadChunkBuildingIDs.Add(CurrentID);
							ConnectUnLoadCoords.Add(*FoundCoords2);
						}
					}
				}
			}
			
		}
	}

	for (const auto& Pair : ResourceMap)
	{
		const FResourceNode& Node = Pair.Value;

		if (Node.OwnerChunk == Chunk)
		{
			RemoveResourceIDs.Add(Pair.Key);
		}
	
	}

	TArray<FAttachBuildKey> RemoveAttachKeys;

	for (const auto& Pair : HISMAttachBuildActors) {
		if (RemoveBuildingIDs.Contains(Pair.Key.ParentBuildingID))
			RemoveAttachKeys.Add(Pair.Key);
	}

	for (const FAttachBuildKey& Key : RemoveAttachKeys) {
		ABuildable** FoundBuildable = HISMAttachBuildActors.Find(Key);

		if (FoundBuildable && IsValid(*FoundBuildable))
			(*FoundBuildable)->Destroy();
		HISMAttachBuildActors.Remove(Key);
	}

	for(int32 RemoveID : RemoveBuildingIDs)
		BuildingMap.Remove(RemoveID);

	for (int32 RemoveID : RemoveResourceIDs)
		ResourceMap.Remove(RemoveID);

	Chunk->ClearAll();
	Chunk->Destroy();

	LoadChunks.Remove(Coords);
	

	float Elapse = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

	ChunkStreamingStats.ChunkUnloadCount++;
	ChunkStreamingStats.TotalChunkUnloadTime += Elapse;
	ChunkStreamingStats.AverageChunkUnloadTime = ChunkStreamingStats.ChunkUnloadCount > 0? ChunkStreamingStats.TotalChunkUnloadTime 
		/ ChunkStreamingStats.ChunkUnloadCount: 0.f;

	if (bLogChunkStreaming)
	{
		UE_LOG(LogTemp,Log,TEXT("[ChunkStreaming] Unload Chunk %s / %.3f ms / Buildings:%d Resources:%d"),*ChunkCoordsString(Coords),
			Elapse,RemoveBuildingIDs.Num(),RemoveResourceIDs.Num());
	}

	return true;
}
*/
void AChunkManager::RemoveBuildingIDSaveCachedData(int32 RemoveBuildingID)
{
	if (!RuntimeSaveGame)
		return;

	for (FChunkSaveData& ChunkData : RuntimeSaveGame->Chunks)
	{
		for (int32 i = ChunkData.Buildings.Num() - 1; i >= 0; i--) {

			if (ChunkData.Buildings[i].ID == RemoveBuildingID) {
				ChunkData.Buildings.RemoveAt(i);
				continue;
			}

			TArray<FName> RemoveSocketNames;

			for (const auto& SocketPair : ChunkData.Buildings[i].SocketMap) {
				if (SocketPair.Value == RemoveBuildingID)
					RemoveSocketNames.Add(SocketPair.Key);
			}

			for (const FName& SocketName : RemoveSocketNames)
				ChunkData.Buildings[i].SocketMap.Remove(SocketName);
		}
		for (int32 i = ChunkData.AttachBuildables.Num() - 1; i >= 0; --i)
		{
			if (ChunkData.AttachBuildables[i].ParentBuildingID == RemoveBuildingID)
				ChunkData.AttachBuildables.RemoveAt(i);
		}
	}
	RebuildChunkDataIndex();
}

void AChunkManager::RemoveResourceIDSaveCachedData(int32 RemoveResourceID)
{
	if (!RuntimeSaveGame)
		return;

	for (FChunkSaveData& ChunkData : RuntimeSaveGame->Chunks)
	{
		for (int32 i = ChunkData.Resources.Num() - 1; i >= 0; --i)
		{
			if (ChunkData.Resources[i].ID == RemoveResourceID)
			{
				ChunkData.Resources.RemoveAt(i);
			}
		}
	}

	RebuildChunkDataIndex();
}

bool AChunkManager::SaveLoadChunkCache(const FChunkCoords& Coords)
{
	double StartSeconds = FPlatformTime::Seconds();

	FChunkSaveData ChunkData;

	FChunkCoords MutableCoords = Coords;

	bool bHasData = BuildChunkSaveData(MutableCoords, ChunkData);

	if (bHasData)
		UpsertSaveCacheChunkData(Coords, ChunkData);
	else
		RemoveSaveCacheChunkData(Coords);

	DirtyChunks.Remove(Coords);

	float Elapse = static_cast<float>((FPlatformTime::Seconds() - StartSeconds) * 1000.0);

	ChunkStreamingStats.ChunkSaveCount++;
	ChunkStreamingStats.TotalChunkSaveTime += Elapse;
	ChunkStreamingStats.AverageChunkSaveTime = ChunkStreamingStats.ChunkSaveCount > 0 ? 
		ChunkStreamingStats.TotalChunkSaveTime / ChunkStreamingStats.ChunkSaveCount: 0.f;

	if (bLogChunkStreaming)
		UE_LOG(LogTemp,Log,TEXT("[ChunkStreaming] Save Chunk Cache %s / %.3f ms / HasData:%s"),*ChunkCoordsString(Coords),
			Elapse,bHasData ? TEXT("true") : TEXT("false"));


	return bHasData;
}

bool AChunkManager::SpawnAttachBuildableSaveData(const FAttachBuildableSaveData& SaveData)
{
	if (!BuildingMap.Contains(SaveData.ParentBuildingID))
		return false;

	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());

	if (!Inst)
		return false;

	const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(SaveData.BuildDefinitionName);

	if (!Info)
		return false;
	UBuildAssetSubsystem* AssetSubsystem = GetGameInstance()->GetSubsystem<UBuildAssetSubsystem>();

	if (!AssetSubsystem)
		return false;

	FBuildAssets Assets;

	if (!AssetSubsystem->ResolveSync(SaveData.BuildDefinitionName, Assets))
		return false;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABuildable* Buildable = GetWorld()->SpawnActor<ABuildable>(Assets.BuildClass,SaveData.Transform.GetLocation(),SaveData.Transform.Rotator(),Params);

	if (!Buildable)
		return false;

	Buildable->SetBuildDefinition(Info, SaveData.BuildDefinitionName, Assets.DestructibleMesh, Assets.StaticMesh);
	Buildable->SetAttachParentInfo(SaveData.ParentBuildingID, SaveData.ParentSocketName);

	Buildable->SetActorTransform(SaveData.Transform);
	Buildable->ApplyMaterialFromBaseMaterials();
	Buildable->SetDurability(SaveData.Durability);

	bool Add = AddHISMAttachBuildActor(SaveData.ParentBuildingID, SaveData.ParentSocketName, Buildable);

	if (!Add) {
		Buildable->Destroy();
		return false;
	}
	return true;;
}

TSet<FChunkCoords> AChunkManager::BuildWantChunkSet(const FChunkCoords& CenterCoords)
{
	TSet<FChunkCoords> SetChunk;

	for (int32 x = -StreamingRadius; x <= StreamingRadius; x++)
	{
		for (int32 y = -StreamingRadius; y <= StreamingRadius; y++)
		{
			SetChunk.Add(FChunkCoords(CenterCoords.X + x, CenterCoords.Y + y));
		}
	}
	return SetChunk;
}
TSet<FChunkCoords> AChunkManager::BuildConnectedWantedChunkSet(const TSet<FChunkCoords>& WantedChunks)
{
	TSet<FChunkCoords> FinalWantedChunks = WantedChunks;
	TQueue<FChunkCoords> ChunkQueue;
	for (const FChunkCoords& Coords : WantedChunks)
	{
		ChunkQueue.Enqueue(Coords);
	}
	while (!ChunkQueue.IsEmpty())
	{

		FChunkCoords CurrentChunk;
		ChunkQueue.Dequeue(CurrentChunk);

		FChunkSaveData* ChunkData = FindSaveCacheChunkData(CurrentChunk);
		if (!ChunkData)
			continue;
		for (const FBuildingSaveData& Building : ChunkData->Buildings) {
			for (const auto& pair : Building.SocketMap) {
				int32 ID = pair.Value;
				if (ID < 0)
					continue;
				const FChunkCoords* ConnectedCoords =ChunkBuildingDataIndexCoords.Find(ID);
				if (!ConnectedCoords)
					continue;
				if (!FinalWantedChunks.Contains(*ConnectedCoords))
				{
					FinalWantedChunks.Add(*ConnectedCoords);
					ChunkQueue.Enqueue(*ConnectedCoords);
				}
			}
		}

	}


	return FinalWantedChunks;
}



void AChunkManager::FlushDirtyChunksSaveCache()
{
	TArray<FChunkCoords> DirtyList = DirtyChunks.Array();

	for (const FChunkCoords& DirtyCoords : DirtyList)
	{
		if (LoadChunks.Contains(DirtyCoords))
			SaveLoadChunkCache(DirtyCoords);
		else
			DirtyChunks.Remove(DirtyCoords);
	}
}

void AChunkManager::UpsertSaveCacheChunkData(const FChunkCoords& Coords, const FChunkSaveData& ChunkData)
{
	if (!RuntimeSaveGame)
		RuntimeSaveGame = Cast<UChunkSaveGame>(UGameplayStatics::CreateSaveGameObject(UChunkSaveGame::StaticClass()));

	if (!RuntimeSaveGame)
		return;
	int32* FoundIndex = ChunkDataIndexCoords.Find(Coords);

	if (FoundIndex)
	{
		if (RuntimeSaveGame->Chunks.IsValidIndex(*FoundIndex)) {
			RuntimeSaveGame->Chunks[*FoundIndex] = ChunkData;
			RebuildChunkDataIndex();
			return;
		}
	}
	int32 NewIndex = RuntimeSaveGame->Chunks.Add(ChunkData);
	RebuildChunkDataIndex();
}

FChunkSaveData* AChunkManager::FindSaveCacheChunkData(const FChunkCoords& Coords)
{
	if (!RuntimeSaveGame)
		return nullptr;

	const int32* FoundIndex = ChunkDataIndexCoords.Find(Coords);

	if (!FoundIndex)
		return nullptr;

	if (!RuntimeSaveGame->Chunks.IsValidIndex(*FoundIndex))
		return nullptr;
	return &RuntimeSaveGame->Chunks[*FoundIndex];
}

void AChunkManager::EnvironmentActorsToHISM() {
	TArray<AActor*> FoliageActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnvironments::StaticClass(), FoliageActors);
	
	for (AActor* Actor : FoliageActors)
	{
		AEnvironments* FoliageActor = Cast<AEnvironments>(Actor);
		if (!FoliageActor) continue;
		FTransform Transform = FoliageActor->GetTransform();
		Transform.SetLocation(FoliageActor->GetActorLocation());

		AddResourceInstanceDefinitionName(FoliageActor->GetDropInfoString(), Transform);
		FoliageActor->Destroy();
		/*
		UStaticMesh* Mesh = FoliageActor->GetMesh();

		FTransform Trans = FoliageActor->GetTransform();
		int32 HP = FoliageActor->GetHP();
		EnvironmentsType Type = FoliageActor->GetType();
		FString String = FoliageActor->GetDropInfoString();
		AddResourceInstanceID(Mesh, Trans,HP,Type, String); 
		
		*/
	}
	
}
void AChunkManager::EnvironmentActorsClearAll() {
	TArray<AActor*> FoliageActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnvironments::StaticClass(), FoliageActors);
	for (AActor* Actor : FoliageActors)
	{
		AEnvironments* FoliageActor = Cast<AEnvironments>(Actor);
		if (!FoliageActor) continue;
		FoliageActor->Destroy();	
	}
}
void AChunkManager::HISMToEnvironmentActors() {


}
void AChunkManager::ClearBuildOnly()
{
	for (auto& Object : HISMAttachBuildActors) {
		if (IsValid(Object.Value))
			Object.Value->Destroy();
	}
	HISMAttachBuildActors.Empty();

	TArray<FChunkCoords> RemoveCoords;

	for (auto& Pair : LoadChunks) {
		if (!IsValid(Pair.Value))
		{
			RemoveCoords.Add(Pair.Key);
			continue;
		}
		Pair.Value->ClearBuildingHISMCompMap();

		if (!Pair.Value->HasResourceHISM())
		{
			Pair.Value->Destroy();
			RemoveCoords.Add(Pair.Key);
		}
	}
	for (const FChunkCoords& Coords : RemoveCoords)
	{
		LoadChunks.Remove(Coords);
	}

	BuildingMap.Empty();
	BuildingID = 0;
}
// Called every frame
void AChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (!bEnableChunkStreaming || !bStreamingStart)
		return;


	StreamingUpdateElapse += DeltaTime;

	if (StreamingUpdateElapse >= StreamingUpdateInterval)
	{
		StreamingUpdateElapse = 0.f;
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (!PlayerPawn)
			return;

		UpdateChunkStreamingAroundLocation(PlayerPawn->GetActorLocation(), false);
	}
}

AChunkActor* AChunkManager::GetChunk(FVector Location)
{
	

	FChunkCoords Coords = GetChunkCoordsLocation(Location);

	
	return GetOrLoadChunkCoords(Coords);
}

void AChunkManager::ClearAll()
{
	for (auto& Object : HISMAttachBuildActors)
	{
		if (IsValid(Object.Value))
			Object.Value->Destroy();
	}
	HISMAttachBuildActors.Empty();
	for (auto Chunk : LoadChunks) {
		if (IsValid(Chunk.Value)) {
			Chunk.Value->ClearAll();
			Chunk.Value->Destroy();
		}
	}
	LoadChunks.Empty();
	BuildingMap.Empty();
	ResourceMap.Empty();
	BuildingID = 0;
	ResourceID = 1000000;
	DirtyChunks.Empty();
	GraphDirtyChunks.Empty();
}



bool AChunkManager::UpgradeBuilding(int32 ID,FString Name,UStaticMesh* NewMesh, UDestructibleMesh* NewDestructibleMesh, int32 NewDurability, int32 NewMaxDurability, EnvironmentsType NewType)
{
	if (!BuildingMap.Contains(ID))
		return false;

	FBuildingNode& Node = BuildingMap[ID];

	if (!Node.OwnerChunk || !Node.Mesh)
		return false;
	FTransform Transform;
	EBuildType BuildType = Node.BuildType;
	
	auto* HISM = Node.OwnerChunk->FindHISM(Node.Mesh,EObType::Building);
	if (!HISM)return false;
	HISM->GetInstanceTransform(Node.InstanceIndex, Transform,true);
	//PrintViewport(5.f, FColor::Red, FString::Printf(TEXT("NodeLocation: %f %f %f"), Transform.GetLocation().X, Transform.GetLocation().Y, Transform.GetLocation().Z));


	int32 MoveID = Node.OwnerChunk->RemoveInstance(Node.Mesh, Node.InstanceIndex, EObType::Building);
	if (MoveID != -1 && BuildingMap.Contains(MoveID))
	{
		BuildingMap[MoveID].InstanceIndex = Node.InstanceIndex;
	}
	int32 NewIndex = Node.OwnerChunk->AddInstance(NewMesh, Transform, ID);
	if (NewIndex == -1)
		return false;
	Node.Mesh = NewMesh;
	Node.BuildDefinitionName = Name;
	Node.DestructibleMesh = NewDestructibleMesh;
	Node.InstanceIndex = NewIndex;
	Node.Durability = NewDurability;
	Node.MaxDurability = NewMaxDurability;
	Node.MaterialType = NewType;
	Node.BuildType = BuildType;
	MarkChunkDirtyBuildingID(ID);
	return true;
}

void AChunkManager::DestroyBuilding(int32 ID)
{
	if (!BuildingMap.Contains(ID))
		return;

	TArray<int32> StartNodes = BuildingMap[ID].ConnectedBuildingIDs;
	
	DisconnectBuilding(ID);
	RemoveOnlyBuilding(ID);
	TSet<int32> Checked;
	
	for (int32 StartID : StartNodes) 
	{
		if (!BuildingMap.Contains(StartID))
			continue;
		if (Checked.Contains(StartID))
			continue;
		TSet<int32> RemoveIDs;
		TQueue<int32> Q;
		bool DestroyBuild = true;

		Q.Enqueue(StartID);
		RemoveIDs.Add(StartID);
		Checked.Add(StartID);

		while (!Q.IsEmpty()) 
		{
			int32 CurrentID;
			Q.Dequeue(CurrentID);

			if (!BuildingMap.Contains(CurrentID))
				continue;
		
			FBuildingNode& CurrentNode = BuildingMap[CurrentID];

			if (CurrentNode.IsAnchor)
				DestroyBuild = false;

			for (int32 NextID : CurrentNode.ConnectedBuildingIDs) {
				if (!BuildingMap.Contains(NextID)) {
					continue;
				}
				if (Checked.Contains(NextID))
					continue;
				Q.Enqueue(NextID);
				RemoveIDs.Add(NextID);
				Checked.Add(NextID);
			}
		}

		if (DestroyBuild) 
		{
			for(int32 RemoveID : RemoveIDs)
				DisconnectBuilding(RemoveID);

			for (int32 RemoveID : RemoveIDs)
				RemoveOnlyBuilding(RemoveID);
			
		}
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
		return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerPawn);
	if (!Player)
		return;
	Player->CheckUpdateOverlaps();

}


void AChunkManager::DisconnectBuilding(int32 ID)
{
	if (!BuildingMap.Contains(ID))
		return;
	FBuildingNode& Node = BuildingMap[ID];


	for (int32 ConnectedID : Node.ConnectedBuildingIDs) {
		if (!BuildingMap.Contains(ConnectedID))
			continue;
		FBuildingNode& ConnectedNode = BuildingMap[ConnectedID];
		if (ConnectedNode.ConnectedBuildingIDs.Contains(ID))
			ConnectedNode.ConnectedBuildingIDs.Remove(ID);
		
		int32 Check=0;
		FName RemoveKey;
		for (auto Pair : ConnectedNode.SocketMap) {
			if (Pair.Value == ID) {
				Check++;
				RemoveKey = Pair.Key;
			}
		}
		
		check(Check <= 1);
		if (Check == 1) {
			ConnectedNode.SocketMap.Remove(RemoveKey);
		}

		MarkChunkDirtyBuildingID(ConnectedID);
		MarkGraphChunkDirtyBuildingID(ConnectedID);
	}
	Node.ConnectedBuildingIDs.Empty();
	Node.SocketMap.Empty();
	MarkChunkDirtyBuildingID(ID);
	MarkGraphChunkDirtyBuildingID(ID);
}

void AChunkManager::RemoveOnlyBuilding(int32 ID)
{
	if (!BuildingMap.Contains(ID))
		return;

	FBuildingNode Node = BuildingMap[ID];
	FTransform Transform;
	auto* HISM = Node.OwnerChunk->FindHISM(Node.Mesh, EObType::Building);
	if (!HISM)return;
	HISM->GetInstanceTransform(Node.InstanceIndex, Transform, true);

	FChunkCoords DirtyCoord = GetChunkCoordsLocation(Transform.GetLocation());

	if (Node.DestructibleMesh)
	{
		ADestructibleBuildable* DestructibleMesh = GetWorld()->SpawnActorDeferred<ADestructibleBuildable>(ADestructibleBuildable::StaticClass(), Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (DestructibleMesh)
		{
			DestructibleMesh->SetDestructibleMesh(Node.DestructibleMesh);
			DestructibleMesh->FinishSpawning(Transform);
		}
	}


	TArray<FAttachBuildKey> DestroyKeys;

	for (auto& Pair : HISMAttachBuildActors)
	{
		if (Pair.Key.ParentBuildingID == ID)
		{
			DestroyKeys.Add(Pair.Key);
		}
	}

	for (const FAttachBuildKey& Key : DestroyKeys)
	{
		ABuildable** Buildable = HISMAttachBuildActors.Find(Key);
		{
			if (Buildable && IsValid(*Buildable))
			{
				(*Buildable)->BuildDestroy();
			}
		}
	}
	if (Node.OwnerChunk) {
		int32 MoveID = Node.OwnerChunk->RemoveInstance(Node.Mesh, Node.InstanceIndex, EObType::Building);
		if (MoveID != -1 && BuildingMap.Contains(MoveID))
		{
			BuildingMap[MoveID].InstanceIndex = Node.InstanceIndex;
		}
	}
	BuildingMap.Remove(ID);
	RemoveBuildingIDSaveCachedData(ID);
	MarkChunkDirty(DirtyCoord);
	MarkGraphChunkDirty(DirtyCoord);
}
void AChunkManager::DestroyResource(int32 ID)
{
	if (!ResourceMap.Contains(ID) || !ResourceTableInfo || !ItemBox)
		return;

	FResourceNode Node = ResourceMap[ID];
	if (!Node.OwnerChunk || !Node.Mesh)
		return;
	FTransform Transform;
	UHierarchicalInstancedStaticMeshComponent* HISM = Node.OwnerChunk->FindHISM(Node.Mesh, EObType::Resource);


	if (!HISM)return;


	HISM->GetInstanceTransform(Node.InstanceIndex, Transform, true);

	FChunkCoords DirtyCoords = GetChunkCoordsLocation(Transform.GetLocation());

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector Location = Transform.GetLocation();
	Location.Z += 100.f;
	Transform.SetLocation(Location);

	const FResourceTableInfo* Info = ResourceTableInfo->FindRow<FResourceTableInfo>(*Node.ResourceDefinitionName, "");

	AItemBox* Box = GetWorld()->SpawnActor<AItemBox>(ItemBox->GetDefaultObject()->GetClass(), Transform, param);
	//아이템 박스 생성후 스폰
	if (Box && Info)
	{
		for (int32 i = 0; i < Info->m_DropItems.Num(); i++)
		{
			int32 aMount = FMath::RandRange(Info->m_DropItems[i].MinaMount, Info->m_DropItems[i].MaxaMount);
			if (aMount <= 0)
				continue;
			FBoxItems Item;
			Item.aMount = aMount;
			Item.ItemName = Info->m_DropItems[i].ItemName;
			Box->AddItem(Item);
		}
	}

	if (Node.OwnerChunk) {
		int32 MoveID = Node.OwnerChunk->RemoveInstance(Node.Mesh, Node.InstanceIndex, EObType::Resource);
		if (MoveID != -1 && ResourceMap.Contains(MoveID))
		{
			ResourceMap[MoveID].InstanceIndex = Node.InstanceIndex;
		}
	}
	ResourceMap.Remove(ID);

	RemoveResourceIDSaveCachedData(ID);
	MarkChunkDirty(DirtyCoords);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
		return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerPawn);
	if (!Player)
		return;
	Player->CheckUpdateOverlaps();
}

bool AChunkManager::AddSocket(int32 ID, int32 ConnectID, FName SocketName, FName ConnectSocketName)
{
	if (BuildingMap.Contains(ID) && BuildingMap.Contains(ConnectID) && CheckSocket(ConnectID, ConnectSocketName) && CheckSocket(ID, SocketName)) {

		BuildingMap[ID].ConnectedBuildingIDs.AddUnique(ConnectID);
		BuildingMap[ID].SocketMap.Add(SocketName, ConnectID);
		BuildingMap[ConnectID].ConnectedBuildingIDs.AddUnique(ID);
		BuildingMap[ConnectID].SocketMap.Add(ConnectSocketName,ID);
		MarkChunkDirtyBuildingID(ID);
		MarkChunkDirtyBuildingID(ConnectID);
		MarkGraphChunkDirtyBuildingID(ID);
		MarkGraphChunkDirtyBuildingID(ConnectID);
		return true;
	}
	return false;
}


bool AChunkManager::CheckSocket(int32 ID, FName SocketName)
{
	if (!BuildingMap.Find(ID))
		return false;
	return !BuildingMap[ID].SocketMap.Contains(SocketName);
}


bool AChunkManager::AddHISMAttachBuildActor(int32 ID, FName SocketName, ABuildable* Buildable)
{
	if (Buildable && BuildingMap.Contains(ID) && CheckSocket(ID, SocketName)) {

		FAttachBuildKey Key(ID, SocketName);
		if (HISMAttachBuildActors.Contains(Key))
			return false;

		//BuildingMap[ID].SocketMap.Add(SocketName, -1);
		HISMAttachBuildActors.Add(Key,Buildable);
		Buildable->SetAttachParentInfo(ID, SocketName);

		MarkChunkDirtyBuildingID(ID);
		//MarkGraphChunkDirtyBuildingID(ID);
		return true;
	}
	return false;
}

bool AChunkManager::RemoveHISMAttachBuildActor(int32 ID, FName SocketName) {
	FAttachBuildKey Key(ID, SocketName);
	
	if (!HISMAttachBuildActors.Contains(Key))
		return false;
	
	HISMAttachBuildActors.Remove(Key);
	MarkChunkDirtyBuildingID(ID);
	MarkGraphChunkDirtyBuildingID(ID);
	return true;
	/*
	if (BuildingMap.Contains(ID)) {
		FName RemoveSocket;

		for (auto& Pair : BuildingMap[ID].SocketMap) {
			if (Pair.Value == -1) {
				RemoveSocket = Pair.Key;
				break;
			}
		}
		if (!RemoveSocket.IsNone())
			BuildingMap[ID].SocketMap.Remove(RemoveSocket);

		MarkChunkDirtyBuildingID(ID);
	}
	int32 Removed= HISMAttachObjects.Remove(ID);
	if (Removed > 0)
		return true;
	else
		return false;
	*/
}

int32 AChunkManager::AddBuildingInstanceID(UStaticMesh* Mesh, const FTransform& Transform,FString Name, UDestructibleMesh* DestructibleMesh, int32 Durability, int32 MaxDurability, EnvironmentsType Type, EBuildType BType, bool IsAnchor, int32 ID)
{
	int32 NewID = (ID == INDEX_NONE) ? BuildingID : ID;

	
	AChunkActor* Target = GetChunk(Transform.GetLocation());
	if (!Target)
		return INDEX_NONE;
	int32 NewIndex = Target->AddInstance(Mesh, Transform, NewID);
	if (NewIndex == INDEX_NONE)
		return INDEX_NONE;

	FBuildingNode NewNode(Target, NewIndex, Name, Mesh, DestructibleMesh, Durability, MaxDurability, Type, BType, IsAnchor);

	//AddParent 는 부모 여러개 있을수 있으니깐 싹다 해줘야되니깐 여기말고 생성시킬때 따로 처리
	BuildingMap.Add(NewID, NewNode);

	if (ID == INDEX_NONE) {
		BuildingID++;
	}
	else {
		BuildingID = FMath::Max(ID + 1,BuildingID);
	}

	MarkChunkDirtyLocation(Transform.GetLocation());
	return NewID;
}

int32 AChunkManager::AddResourceInstanceID(UStaticMesh* Mesh, const FTransform& Transform, int32 Hp, EnvironmentsType Type, FString Name, int32 ID)
{
	int32 NewID = (ID == INDEX_NONE) ? ResourceID : ID;

	AChunkActor* Target = GetChunk(Transform.GetLocation());
	
	if (!Target)
		return INDEX_NONE;
	int32 NewIndex = -1;

	
	if (Target) {
		NewIndex = Target->AddInstance(Mesh, Transform, NewID);
	}
	if (NewIndex==-1)
		return INDEX_NONE;
	
	FResourceNode NewNode(Target, NewIndex, Mesh, Hp, Type, Name);
	ResourceMap.Add(NewID, NewNode);

	if (ID == INDEX_NONE)
	{
		ResourceID++;
	}
	else
	{
		ResourceID = FMath::Max(ResourceID, ID + 1);
	}

	MarkChunkDirtyLocation(Transform.GetLocation());
	return NewID;
}

bool AChunkManager::BuildChunkSaveData(FChunkCoords& Coords, FChunkSaveData& OutData)
{
	OutData.Coords = Coords;
	OutData.Buildings.Empty();
	OutData.Resources.Empty();
	OutData.AttachBuildables.Empty();

	for (auto& Pair : BuildingMap) {
		FTransform Transform;
		if (!GetBuildingTransform(Pair.Key, Transform))
			continue;
		if (!(GetChunkCoordsLocation(Transform.GetLocation()) == Coords))
		continue;

		FBuildingNode& Node = Pair.Value;
		FBuildingSaveData SaveData;
		SaveData.ID = Pair.Key;
		SaveData.BuildDefinitionName = Node.BuildDefinitionName;
		SaveData.Durability = Node.Durability;
		SaveData.Transform = Transform;
		for (auto& SocketPair : Node.SocketMap) {
			if (SocketPair.Value >= 0) {
				SaveData.SocketMap.Add(SocketPair.Key, SocketPair.Value);
			}
		}
		OutData.Buildings.Add(SaveData);
	}

	for (auto& Pair : ResourceMap) {
		FTransform Transform;
		if (!GetResourceTransform(Pair.Key, Transform))
			continue;
		if (!(GetChunkCoordsLocation(Transform.GetLocation()) == Coords))
			continue;

		FResourceNode& Node = Pair.Value;
		FResourceSaveData SaveData;
		SaveData.ID = Pair.Key;
		SaveData.ResourceDefinitionName = Node.ResourceDefinitionName;
		SaveData.Hp = Node.Hp;
		SaveData.ResourceType = Node.ResourceType;
		SaveData.Transform = Transform;
		OutData.Resources.Add(SaveData);
	}

	for (auto& Pair : HISMAttachBuildActors) {
		const FAttachBuildKey& Key = Pair.Key;
		ABuildable* Buildable = Pair.Value;

		if (!IsValid(Buildable))
			continue;
		if (!BuildingMap.Contains(Key.ParentBuildingID))
			continue;

		FTransform Transform;

		if (!GetBuildingTransform(Key.ParentBuildingID, Transform))
			continue;

		if (!(GetChunkCoordsLocation(Transform.GetLocation()) == Coords))
		continue;

		FAttachBuildableSaveData SaveData;

		SaveData.ParentBuildingID = Key.ParentBuildingID;
		SaveData.BuildDefinitionName = Buildable->GetBuildName();
		SaveData.ParentSocketName = Key.ParentSocketName;
		SaveData.Transform = Buildable->GetActorTransform();
		SaveData.Durability = Buildable->GetDurability();
		OutData.AttachBuildables.Add(SaveData);
	}
	return OutData.Buildings.Num() >0 || OutData.Resources.Num() > 0 || OutData.AttachBuildables.Num() > 0;
}


int32 AChunkManager::AddBuildingInstanceDefinitionName(FString BuildDefinitionName,const FTransform& Transform, int32 ID, int32 Durability)
{

	UBuildAssetSubsystem* Resolver =GetWorld()->GetGameInstance()->GetSubsystem<UBuildAssetSubsystem>();
	if (!Resolver)
		return INDEX_NONE;
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
	if (!Inst)return INDEX_NONE;

	const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(BuildDefinitionName);

	if (!Info) return INDEX_NONE;

	FBuildAssets Assets;
	if (!Resolver->ResolveSync(BuildDefinitionName, Assets))
		return INDEX_NONE;

	int32 FinalDurability = (Durability == INDEX_NONE) ? Info->Durability : Durability;

	return AddBuildingInstanceID(Assets.StaticMesh, Transform, BuildDefinitionName, Assets.DestructibleMesh, FinalDurability, Info->MaxDurability, Info->MaterialType, Info->BuildType, Info->Anchor, ID);
}

bool AChunkManager::UpgradeBuildingDefinitionName(int32 ID,FString BuildDefinitionName)
{
	UBuildAssetSubsystem* Resolver = GetWorld()->GetGameInstance()->GetSubsystem<UBuildAssetSubsystem>();
	if (!Resolver)
		return false;
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
	if (!Inst)
		return false;

	const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(BuildDefinitionName);
	if (!Info)
		return false;

	FBuildAssets Assets;
	if (!Resolver->ResolveSync(BuildDefinitionName, Assets))
		return false;
	if (UpgradeBuilding(ID, BuildDefinitionName, Assets.StaticMesh, Assets.DestructibleMesh, Info->Durability, Info->MaxDurability, Info->MaterialType)) {
		FBuildingNode* Node = BuildingMap.Find(ID);
		if (Node)
		{
			Node->BuildType = Info->BuildType;
			Node->IsAnchor = Info->Anchor;
			Node->BuildDefinitionName = BuildDefinitionName;
		}
		return true;
	}

	return false;
	
}

void AChunkManager::SaveWorld()
{
	if (bEnableChunkStreaming || bStreamingStart)
	{
		SaveWorldDirty();
		return;
	}

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
		SaveWorldDirty();
	else
		SaveWorldFull();
}
void AChunkManager::SaveWorldFull()
{

	if (bEnableChunkStreaming || bStreamingStart)
	{
		if (!RuntimeSaveGame)
		{
			if (!LoadRuntimeSaveCache())
				return;
		}

		TArray<FChunkCoords> LoadedCoords;
		LoadChunks.GetKeys(LoadedCoords);

		for (const FChunkCoords& Coords : LoadedCoords)
		{
			SaveLoadChunkCache(Coords);
		}

		SaveRuntimeSaveCacheSlot();

		RecalculateStreamingStats();
		LogChunkStreamingStats(TEXT("SaveWorldFull_Streaming"));

		return;
	}

	UChunkSaveGame* SaveGame = Cast<UChunkSaveGame>(UGameplayStatics::CreateSaveGameObject(UChunkSaveGame::StaticClass()));

	if (!SaveGame)return;


	SaveGame->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	SaveGame->SaveVersion = 1;
	SaveGame->NextBuildingID = BuildingID;
	SaveGame->NextResourceID = ResourceID;

	const double TotalStartSeconds = FPlatformTime::Seconds();

	int32 BuildChunkSaveDataCallCount = 0;
	int32 SaveDataUpdatedChunkCount = 0;

	double BuildChunkSaveDataTotalMs = 0.0;


	for (auto& Pair : LoadChunks) {
		FChunkSaveData ChunkData;

		BuildChunkSaveDataCallCount++;
		const double BuildStartSeconds = FPlatformTime::Seconds();

		const bool bBuildChunk = BuildChunkSaveData(Pair.Key, ChunkData);
		BuildChunkSaveDataTotalMs +=
			(FPlatformTime::Seconds() - BuildStartSeconds) * 1000.0;
		if (bBuildChunk) {
			SaveGame->Chunks.Add(ChunkData);
			SaveDataUpdatedChunkCount++;

		}

	}
	const double SaveSlotStartSeconds = FPlatformTime::Seconds();

	bool Saved= UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, SaveUserIndex);
	const double SaveSlotMs =
		(FPlatformTime::Seconds() - SaveSlotStartSeconds) * 1000.0;

	const double TotalMs =
		(FPlatformTime::Seconds() - TotalStartSeconds) * 1000.0;

	UE_LOG(LogTemp, Log,
		TEXT("[DirtySaveBenchmark] Mode=Full TotalChunks=%d DirtyChunks=- BuildCalls=%d UpdatedChunks=%d BuildDataTime=%.3f ms SaveSlotTime=%.3f ms SaveWorldTime=%.3f ms Saved=%s"),
		LoadChunks.Num(),
		BuildChunkSaveDataCallCount,
		SaveDataUpdatedChunkCount,
		BuildChunkSaveDataTotalMs,
		SaveSlotMs,
		TotalMs,
		Saved ? TEXT("true") : TEXT("false"));
	if (Saved) {
		DirtyChunks.Empty();
		GraphDirtyChunks.Empty();
	}
}

void AChunkManager::SaveWorldDirty()
{
	if (bEnableChunkStreaming || bStreamingStart)
	{
		if (!RuntimeSaveGame)
			if (!LoadRuntimeSaveCache())
			{
				UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] SaveWorldDirty failed: RuntimeSaveGame is null."));
				return;
			}
		FlushDirtyChunksSaveCache();
		SaveRuntimeSaveCacheSlot();

		RecalculateStreamingStats();
		LogChunkStreamingStats(TEXT("SaveWorldDirty_Streaming"));

		return;
	}
	if (DirtyChunks.Num() == 0)
		return;


	UChunkSaveGame* SaveGame = nullptr;

	bool Exists = UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex);
	if (!Exists) {
		SaveWorldFull();
		return;
	}
	const double TotalStartSeconds = FPlatformTime::Seconds(); //

	const int32 TotalChunkCount = LoadChunks.Num(); //
	const int32 DirtyChunkCountBeforeSave = DirtyChunks.Num(); //

	int32 BuildChunkSaveDataCallCount = 0;//
	int32 SaveDataUpdatedChunkCount = 0; //

	const double LoadSlotStartSeconds = FPlatformTime::Seconds(); //

	SaveGame = Cast<UChunkSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));


	const double LoadSlotMs =
		(FPlatformTime::Seconds() - LoadSlotStartSeconds) * 1000.0; //

	if (!SaveGame)return;

	const double MapBuildStartSeconds = FPlatformTime::Seconds(); //

	TMap<FChunkCoords, FChunkSaveData> ChunkMap;
	for (FChunkSaveData& ChunkData : SaveGame->Chunks) {
		ChunkMap.Add(ChunkData.Coords, ChunkData);
	}

	const double MapBuildMs =
		(FPlatformTime::Seconds() - MapBuildStartSeconds) * 1000.0; //
	double BuildChunkSaveDataTotalMs = 0.0; //


	TArray<FChunkCoords> DirtyList = DirtyChunks.Array();
	for (FChunkCoords& DirtyCoord : DirtyList) {
		FChunkSaveData NewChunkData;
		FChunkCoords MutableDirtyCoord = DirtyCoord;

		BuildChunkSaveDataCallCount++;//
		const double BuildStartSeconds = FPlatformTime::Seconds(); //
		const bool bBuildChunk = BuildChunkSaveData(MutableDirtyCoord, NewChunkData);

		BuildChunkSaveDataTotalMs +=
			(FPlatformTime::Seconds() - BuildStartSeconds) * 1000.0; //
		if (bBuildChunk)
			ChunkMap.Add(DirtyCoord, NewChunkData);
		else
			ChunkMap.Remove(DirtyCoord);

		SaveDataUpdatedChunkCount++; //
	}
	const double GenerateArrayStartSeconds = FPlatformTime::Seconds(); //

	SaveGame->Chunks.Empty();
	ChunkMap.GenerateValueArray(SaveGame->Chunks);

	const double GenerateArrayMs =
		(FPlatformTime::Seconds() - GenerateArrayStartSeconds) * 1000.0; //

	SaveGame->NextBuildingID = BuildingID;
	SaveGame->NextResourceID = ResourceID;
	SaveGame->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);


	const double SaveSlotStartSeconds = FPlatformTime::Seconds();//

	bool Saved = UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, SaveUserIndex);

	const double SaveSlotMs =
		(FPlatformTime::Seconds() - SaveSlotStartSeconds) * 1000.0;//

	const double TotalMs =
		(FPlatformTime::Seconds() - TotalStartSeconds) * 1000.0; //
	UE_LOG(LogTemp, Log,
		TEXT("[DirtySaveBenchmark] Mode=Dirty TotalChunks=%d DirtyChunks=%d BuildCalls=%d UpdatedChunks=%d LoadSlotTime=%.3f ms MapBuildTime=%.3f ms BuildDataTime=%.3f ms GenerateArrayTime=%.3f ms SaveSlotTime=%.3f ms SaveWorldTime=%.3f ms Saved=%s"),
		TotalChunkCount,
		DirtyChunkCountBeforeSave,
		BuildChunkSaveDataCallCount,
		SaveDataUpdatedChunkCount,
		LoadSlotMs,
		MapBuildMs,
		BuildChunkSaveDataTotalMs,
		GenerateArrayMs,
		SaveSlotMs,
		TotalMs,
		Saved ? TEXT("true") : TEXT("false"));
	if (Saved) {
		DirtyChunks.Empty();
		GraphDirtyChunks.Empty();
	}
}                                 

                                                                                                                                                     

void AChunkManager::LoadWorld()
{
	if (bEnableChunkStreaming)
	{
		StartChunkStreaming();
		return;
	}
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
		return;

	UChunkSaveGame* SaveGame = Cast<UChunkSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

	if (!SaveGame)return;

	SuppressDirtyTracking = true;
	ClearAll();

	for (FChunkSaveData& ChunkData : SaveGame->Chunks) {
		for (FBuildingSaveData& BuildingData : ChunkData.Buildings) {
			AddBuildingInstanceDefinitionName(BuildingData.BuildDefinitionName, BuildingData.Transform, BuildingData.ID, BuildingData.Durability);
		}
		
	
	}
	for (FChunkSaveData& ChunkData : SaveGame->Chunks) {
		for (FBuildingSaveData& BuildingData : ChunkData.Buildings) {
			FBuildingNode* Node = BuildingMap.Find(BuildingData.ID);

			if (!Node)continue;

			Node->SocketMap = BuildingData.SocketMap;
			Node->ConnectedBuildingIDs.Empty();

			for (auto& SocketPair : Node->SocketMap) {
				if (SocketPair.Value >= 0)
				{
					Node->ConnectedBuildingIDs.AddUnique(SocketPair.Value);
				}
			}
		}
	}
	for (FChunkSaveData& ChunkData : SaveGame->Chunks) {
		for (FResourceSaveData& ResourcesData : ChunkData.Resources) {
			AddResourceInstanceDefinitionName(ResourcesData.ResourceDefinitionName, ResourcesData.Transform, ResourcesData.ID, ResourcesData.Hp);
		}
	}
	for (FChunkSaveData& ChunkData : SaveGame->Chunks) {

		for (FAttachBuildableSaveData& SaveData : ChunkData.AttachBuildables) {
			if (!BuildingMap.Contains(SaveData.ParentBuildingID))
				continue;
			UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
			if (!Inst)
				continue;

			const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(SaveData.BuildDefinitionName);
			if (!Info)
				continue;
			UBuildAssetSubsystem* Resolver =
				GetGameInstance()->GetSubsystem<UBuildAssetSubsystem>();
			if (!Resolver)
				continue;
			FBuildAssets Assets;
			if (!Resolver->ResolveSync(SaveData.BuildDefinitionName, Assets))
				continue;

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ABuildable* Buildable = GetWorld()->SpawnActor<ABuildable>(Assets.BuildClass,SaveData.Transform.GetLocation(),SaveData.Transform.Rotator(),Params);
			if (!Buildable)
				continue;
			Buildable->SetBuildDefinition(Info, SaveData.BuildDefinitionName, Assets.DestructibleMesh, Assets.StaticMesh);
			Buildable->SetAttachParentInfo(SaveData.ParentBuildingID, SaveData.ParentSocketName);
			Buildable->SetActorTransform(SaveData.Transform);
			Buildable->ApplyMaterialFromBaseMaterials();
			Buildable->SetDurability(SaveData.Durability);
			bool bAdd = AddHISMAttachBuildActor(SaveData.ParentBuildingID, SaveData.ParentSocketName, Buildable);
			if (!bAdd)
			{
				Buildable->Destroy();
				continue;
			}
		}
	}
	BuildingID = FMath::Max(BuildingID, SaveGame->NextBuildingID);
	ResourceID = FMath::Max(ResourceID, SaveGame->NextResourceID);

	DirtyChunks.Empty();
	GraphDirtyChunks.Empty();
	SuppressDirtyTracking = false;
}

bool AChunkManager::GetResourceTransform(int32 ID, FTransform& Transform)
{
	const FResourceNode* Node = ResourceMap.Find(ID);
	if (!Node || !Node->OwnerChunk || !Node->Mesh)return false;

	UHierarchicalInstancedStaticMeshComponent* HISM = Node->OwnerChunk->FindHISM(Node->Mesh, EObType::Resource);
	if (!HISM) return false;
	if (!(0 <= Node->InstanceIndex && Node->InstanceIndex < HISM->GetInstanceCount())) return false;

	return HISM->GetInstanceTransform(Node->InstanceIndex, Transform, true);
}

bool AChunkManager::GetBuildingTransform(int32 ID, FTransform& Transform)
{
	const FBuildingNode* Node = BuildingMap.Find(ID);
	if (!Node || !Node->OwnerChunk || !Node->Mesh)return false;

	UHierarchicalInstancedStaticMeshComponent* HISM = Node->OwnerChunk->FindHISM(Node->Mesh,EObType::Building);
	if (!HISM) return false;
	if (!(0 <= Node->InstanceIndex && Node->InstanceIndex < HISM->GetInstanceCount())) return false;

	return HISM->GetInstanceTransform(Node->InstanceIndex,Transform,true);
}


void AChunkManager::MarkChunkDirty(const FChunkCoords& Coords)
{
	if (SuppressDirtyTracking) return;
	DirtyChunks.Add(Coords);

}

void AChunkManager::MarkGraphChunkDirty(const FChunkCoords& Coords)
{
	if (SuppressDirtyTracking) return;
	GraphDirtyChunks.Add(Coords);
}

bool AChunkManager::DamageBuilding(int32 ID, int32 Damage)
{
	FBuildingNode* Node = BuildingMap.Find(ID);
	if (!Node)return false;

	Node->Durability -= Damage;


	if (Node->Durability <= 0) 
		DestroyBuilding(ID);
	else
		MarkChunkDirtyBuildingID(ID);

	
	return true;
}

bool AChunkManager::DamageResource(int32 ID, int32 Damage)
{

	FResourceNode* Node = ResourceMap.Find(ID);
	if (!Node)return false;

	Node->Hp -= Damage;
	if (Node->Hp <= 0)DestroyResource(ID);
	else
		MarkChunkDirtyResourceID(ID);
	return true;
}

void AChunkManager::MarkChunkDirtyBuildingID(int32 ID)
{
	FTransform Transform; 
	if (!GetBuildingTransform(ID, Transform))
		return;

	MarkChunkDirty(GetChunkCoordsLocation(Transform.GetLocation()));
}
void AChunkManager::MarkGraphChunkDirtyBuildingID(int32 ID)
{
	FTransform Transform;
	if (!GetBuildingTransform(ID, Transform))
		return;
	MarkGraphChunkDirty(GetChunkCoordsLocation(Transform.GetLocation()));
}



void AChunkManager::MarkChunkDirtyResourceID(int32 ID)
{
	FTransform Transform;
	if (!GetResourceTransform(ID, Transform))
		return;

	MarkChunkDirty(GetChunkCoordsLocation(Transform.GetLocation()));
}
FChunkCoords AChunkManager::GetChunkCoordsLocation(const FVector& Location)
{
	return {
		FMath::FloorToInt(Location.X / ChunkSize),
		FMath::FloorToInt(Location.Y / ChunkSize)
	};
}

void AChunkManager::MarkChunkDirtyLocation(const FVector& Location)
{
	MarkChunkDirty(GetChunkCoordsLocation(Location));
}


void AChunkManager::StartChunkStreaming_Editor() {
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] StartChunkStreaming_Editor can only run during PIE/Game."));
		return;
	}

	StartChunkStreaming();
}
void AChunkManager::StopChunkStreaming_Editor() {
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] StopChunkStreaming_Editor can only run during PIE/Game."));
		return;
	}

	StopChunkStreaming();
}
void AChunkManager::ForceUpdateChunkStreaming_Editor() {
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChunkStreaming] ForceUpdateChunkStreaming_Editor can only run during PIE/Game."));
		return;
	}

	ForceUpdateChunkStreaming();
}


AChunkActor* AChunkManager::FindLoadChunk(const FChunkCoords& Coords)
{
	if (AChunkActor** FoundChunk = LoadChunks.Find(Coords))
	{
		if (IsValid(*FoundChunk))
		{
			return *FoundChunk;
		}

		LoadChunks.Remove(Coords);
	}

	return nullptr;
}

AChunkActor* AChunkManager::GetOrLoadChunkCoords(const FChunkCoords& Coords, bool CreateMissing)
{
	if (AChunkActor* LoadedChunk = FindLoadChunk(Coords))
	{
		return LoadedChunk;
	}

	if (bEnableChunkStreaming)
	{
		if (FindSaveCacheChunkData(Coords))
		{
			if (LoadChunkRuntime(Coords))
			{
				return FindLoadChunk(Coords);
			}

			return nullptr;
		}
	}

	if (!CreateMissing)
	{
		return nullptr;
	}

	return SpawnChunkActorOnly(Coords);
}

AChunkActor* AChunkManager::SpawnChunkActorOnly(const FChunkCoords& Coords) {
	if (!GetWorld())
	{
		return nullptr;
	}

	if (!ChunkActorClass)
	{
		ChunkActorClass = AChunkActor::StaticClass();
	}

	FVector ChunkLocation = FVector(Coords.X * ChunkSize, Coords.Y * ChunkSize, 0.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AChunkActor* NewChunk = GetWorld()->SpawnActor<AChunkActor>(
		ChunkActorClass,
		ChunkLocation,
		FRotator::ZeroRotator,
		Params
		);

	if (NewChunk)
	{
		LoadChunks.Add(Coords, NewChunk);

#if WITH_EDITOR
		NewChunk->SetActorLabel(FString::Printf(TEXT("Chunk X:%d Y:%d"), Coords.X, Coords.Y));
#endif

		return NewChunk;
	}

	return nullptr;
}
void AChunkManager::DrawChunkGrid_Editor()
{
#if WITH_EDITOR
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 이전에 그린 선 정리
	// 주의: 다른 Persistent Debug Line도 같이 지워질 수 있음
	FlushPersistentDebugLines(World);

	const FVector BaseLocation = DebugChunkLocation;
	
	const FChunkCoords CenterCoords = GetChunkCoordsLocation(BaseLocation);


	const int32 MinX = CenterCoords.X - DebugChunkGridRadius;
	const int32 MaxX = CenterCoords.X + DebugChunkGridRadius;
	const int32 MinY = CenterCoords.Y - DebugChunkGridRadius;
	const int32 MaxY = CenterCoords.Y + DebugChunkGridRadius;

	const float Z = BaseLocation.Z + DebugChunkGridZOffset;

	auto DrawChunkRect = [&](const FChunkCoords& Coords, const FColor& Color, float Thickness)
	{
		const float X0 = Coords.X * ChunkSize;
		const float Y0 = Coords.Y * ChunkSize;
		const float X1 = (Coords.X + 1) * ChunkSize;
		const float Y1 = (Coords.Y + 1) * ChunkSize;

		const FVector P0(X0, Y0, Z);
		const FVector P1(X1, Y0, Z);
		const FVector P2(X1, Y1, Z);
		const FVector P3(X0, Y1, Z);

		DrawDebugLine(World, P0, P1, Color, true, -1.f, 0, Thickness);
		DrawDebugLine(World, P1, P2, Color, true, -1.f, 0, Thickness);
		DrawDebugLine(World, P2, P3, Color, true, -1.f, 0, Thickness);
		DrawDebugLine(World, P3, P0, Color, true, -1.f, 0, Thickness);
	};

	for (int32 X = MinX; X <= MaxX; ++X)
	{
		for (int32 Y = MinY; Y <= MaxY; ++Y)
		{
			const FChunkCoords Coords(X, Y);

			FColor DrawColor = DebugChunkGridColor;
			float Thickness = DebugChunkGridThickness;

			if (Coords == CenterCoords)
			{
				DrawColor = DebugCenterChunkColor;
				Thickness = DebugChunkGridThickness + 2.f;
			}
			else if (bDebugHighlightLoadedChunks && LoadChunks.Contains(Coords))
			{
				DrawColor = DebugLoadedChunkColor;
				Thickness = DebugChunkGridThickness + 1.f;
			}

			DrawChunkRect(Coords, DrawColor, Thickness);

			if (bDebugDrawChunkLabel)
			{
				const FVector LabelLocation(
					(X + 0.5f) * ChunkSize,
					(Y + 0.5f) * ChunkSize,
					Z + 30.f
				);

				const FString Label = FString::Printf(TEXT("Chunk (%d, %d)"), X, Y);

				DrawDebugString(
					World,
					LabelLocation,
					Label,
					nullptr,
					FColor::White,
					20.f,
					true
				);
			}
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("[ChunkDebug] Draw Chunk Grid Center=(%d,%d), Radius=%d, ChunkSize=%.1f"),
		CenterCoords.X,
		CenterCoords.Y,
		DebugChunkGridRadius,
		ChunkSize
	);
#endif
}

void AChunkManager::ClearChunkGrid_Editor()
{
#if WITH_EDITOR
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FlushPersistentDebugLines(World);

	UE_LOG(LogTemp, Log, TEXT("[ChunkDebug] Clear Chunk Grid"));
#endif
}

void AChunkManager::ResetChunkStreamingStats_Editor()
{
	ChunkStreamingStats = FChunkStreamingStats();
	RecalculateStreamingStats();

	UE_LOG(LogTemp, Log, TEXT("[ChunkStreaming] Reset stats"));
}

#include "HAL/PlatformMemory.h"

void AChunkManager::PrintMemoryStats_Editor()
{
	const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();

	const double UsedPhysicalMB = Stats.UsedPhysical / 1024.0 / 1024.0;
	const double PeakUsedPhysicalMB = Stats.PeakUsedPhysical / 1024.0 / 1024.0;
	const double UsedVirtualMB = Stats.UsedVirtual / 1024.0 / 1024.0;
	const double PeakUsedVirtualMB = Stats.PeakUsedVirtual / 1024.0 / 1024.0;

	UE_LOG(LogTemp, Log, TEXT("========== [Memory Stats] =========="));
	UE_LOG(LogTemp, Log, TEXT("UsedPhysical      : %.2f MB"), UsedPhysicalMB);
	UE_LOG(LogTemp, Log, TEXT("PeakUsedPhysical  : %.2f MB"), PeakUsedPhysicalMB);
	UE_LOG(LogTemp, Log, TEXT("UsedVirtual       : %.2f MB"), UsedVirtualMB);
	UE_LOG(LogTemp, Log, TEXT("PeakUsedVirtual   : %.2f MB"), PeakUsedVirtualMB);
}

void AChunkManager::Benchmark_SaveWorldFull_Editor() {
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
	
		return;
	}

	if (bEnableChunkStreaming || bStreamingStart)
	{

		return;
	}

	DirtyChunks.Empty();
	GraphDirtyChunks.Empty();

	SaveWorldFull();
}
void AChunkManager::Benchmark_DirtySave1_Editor() {
	RunDirtySaveBenchmark(1);
}
void AChunkManager::Benchmark_DirtySave5_Editor() {
	RunDirtySaveBenchmark(5);

}
void AChunkManager::RunDirtySaveBenchmark(int32 DirtyChunkTargetCount) {
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
	
		return;
	}

	if (bEnableChunkStreaming || bStreamingStart)
	{
		
		return;
	}

	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		
		return;
	}

	// 같은 기준 상태에서 Dirty를 만들기 위해 저장된 월드를 다시 로드
	LoadWorld();

	DirtyChunks.Empty();
	GraphDirtyChunks.Empty();

	if (!MakeDirtyChunksForBenchmark(DirtyChunkTargetCount))
	{
		return;
	}

	SaveWorldDirty();
}
bool AChunkManager::MakeDirtyChunksForBenchmark(int32 TargetChunkCount) {
	TSet<FChunkCoords> TouchedChunks;

	// 건축물 기준으로 먼저 Dirty Chunk 생성
	for (auto& Pair : BuildingMap)
	{
		const int32 BuildingIDKey = Pair.Key;
		FBuildingNode& Node = Pair.Value;

		// 내구도가 1 이하인 건축물은 건드리지 않음.
		// DamageBuilding을 쓰면 파괴될 수 있어서 벤치마크에서는 직접 1만 감소시킴.
		if (Node.Durability <= 1)
			continue;

		FTransform Transform;
		if (!GetBuildingTransform(BuildingIDKey, Transform))
			continue;

		FChunkCoords Coords = GetChunkCoordsLocation(Transform.GetLocation());

		// 같은 청크는 한 번만 Dirty 처리
		if (TouchedChunks.Contains(Coords))
			continue;

		Node.Durability -= 1;

		MarkChunkDirty(Coords);
		TouchedChunks.Add(Coords);

		if (TouchedChunks.Num() >= TargetChunkCount)
			break;
	}

	// 건축물만으로 부족하면 자원도 사용
	if (TouchedChunks.Num() < TargetChunkCount)
	{
		for (auto& Pair : ResourceMap)
		{
			const int32 ResourceIDKey = Pair.Key;
			FResourceNode& Node = Pair.Value;

			if (Node.Hp <= 1)
				continue;

			FTransform Transform;
			if (!GetResourceTransform(ResourceIDKey, Transform))
				continue;

			FChunkCoords Coords = GetChunkCoordsLocation(Transform.GetLocation());

			if (TouchedChunks.Contains(Coords))
				continue;

			Node.Hp -= 1;

			MarkChunkDirty(Coords);
			TouchedChunks.Add(Coords);

			if (TouchedChunks.Num() >= TargetChunkCount)
				break;
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("[DirtySaveBenchmark] MakeDirty Target=%d Actual=%d"),
		TargetChunkCount,
		DirtyChunks.Num());

	return DirtyChunks.Num() == TargetChunkCount;
}