// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ChunkActor.h"
#include "ChunkManager.generated.h"


USTRUCT(BlueprintType)
struct FBuildingNode
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AChunkActor* OwnerChunk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 InstanceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString BuildDefinitionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDestructibleMesh* DestructibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EnvironmentsType MaterialType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EBuildType BuildType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<int32> ConnectedBuildingIDs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<FName, int32> SocketMap;


	FBuildingNode() : OwnerChunk(nullptr), InstanceIndex(-1), BuildDefinitionName(TEXT("")), Mesh(nullptr), DestructibleMesh(nullptr),
		Durability(0), MaxDurability(0), MaterialType(EnvironmentsType::Wood), BuildType(EBuildType::None), IsAnchor(false){}

	FBuildingNode(AChunkActor* Chunk, int32 Index,FString Name,UStaticMesh* Mesh, UDestructibleMesh* DestructibleMesh, int32 Durability, int32 MaxDurability,EnvironmentsType Type,EBuildType Bype,bool Anchor)
		: OwnerChunk(Chunk), InstanceIndex(Index), BuildDefinitionName(Name),Mesh(Mesh), DestructibleMesh(DestructibleMesh), Durability(Durability), MaxDurability(MaxDurability), MaterialType(Type), BuildType (Bype), IsAnchor(Anchor){}
	
};

USTRUCT(BlueprintType)
struct FResourceNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AChunkActor* OwnerChunk;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 InstanceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EnvironmentsType ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString ResourceDefinitionName;
	

	FResourceNode() : OwnerChunk(nullptr), InstanceIndex(-1), Mesh(nullptr), Hp(3), ResourceType(EnvironmentsType::Wood), ResourceDefinitionName(TEXT("")){}

	FResourceNode(AChunkActor* Chunk, int32 Index, UStaticMesh* Mesh,int32 HP, EnvironmentsType Type, FString String)
		: OwnerChunk(Chunk), InstanceIndex(Index), Mesh(Mesh), Hp(HP) ,ResourceType(Type), ResourceDefinitionName(String){}
	

};


USTRUCT(BlueprintType)
struct FAttachBuildKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 ParentBuildingID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName ParentSocketName;

	FAttachBuildKey() : ParentBuildingID(INDEX_NONE), ParentSocketName(NAME_None) {}
	FAttachBuildKey(int32 ID, FName Name) : ParentBuildingID(ID), ParentSocketName(Name) {}
	bool operator==(const FAttachBuildKey& Other) const
	{
		return ParentBuildingID == Other.ParentBuildingID
			&& ParentSocketName == Other.ParentSocketName;
	}
};
FORCEINLINE uint32 GetTypeHash(const FAttachBuildKey& Key)
{
	return HashCombine(
		::GetTypeHash(Key.ParentBuildingID),
		::GetTypeHash(Key.ParentSocketName));
}
USTRUCT(BlueprintType)
struct FChunkCoords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	FChunkCoords():X(0), Y(0){}

	FChunkCoords(int32 InX, int32 InY) : X(InX), Y(InY){}
	FORCEINLINE bool operator==(const FChunkCoords& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

FORCEINLINE uint32 GetTypeHash(const FChunkCoords& Coords)
{
	return HashCombine(GetTypeHash(Coords.X), GetTypeHash(Coords.Y));
}

USTRUCT(BlueprintType)
struct FChunkStreamingStats
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 TotalKnownChunks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ActiveChunks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 UnloadChunks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DirtyChunkCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ActiveBuildingInstances = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ActiveResourceInstances = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ActiveTotalInstances = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ActiveHISMComponents = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ChunkLoadCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ChunkUnloadCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ChunkSaveCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float TotalChunkLoadTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float TotalChunkUnloadTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float TotalChunkSaveTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AverageChunkLoadTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AverageChunkUnloadTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AverageChunkSaveTime = 0.f;
};

class UChunkSaveGame;
struct FChunkSaveData;
struct FAttachBuildableSaveData;

UCLASS()
class PP_API AChunkManager : public AActor
{
	GENERATED_BODY()

		// Sets default values for this actor's properties
public:
	AChunkManager();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float ChunkSize =1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AChunkActor> ChunkActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<FChunkCoords, AChunkActor*> LoadChunks;

	UPROPERTY()
		TMap<int32, FBuildingNode> BuildingMap;

	UPROPERTY()
		TMap<int32, FResourceNode> ResourceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 BuildingID=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 ResourceID= 1000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 TargetID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<FAttachBuildKey, class ABuildable*> HISMAttachBuildActors;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AItemBox> ItemBox;

	UPROPERTY()
		UDataTable* ResourceTableInfo;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString SaveSlotName = TEXT("StreamingTestSave");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 SaveUserIndex = 0;

	TSet<FChunkCoords> DirtyChunks;
	TSet<FChunkCoords> GraphDirtyChunks;
	bool SuppressDirtyTracking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bEnableChunkStreaming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 StreamingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float StreamingUpdateInterval = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bLogChunkStreaming = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bWriteStreamingCSV = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString StreamingCSVFileName = TEXT("ChunkStreamingStats.csv");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FChunkStreamingStats ChunkStreamingStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UChunkSaveGame* RuntimeSaveGame = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FChunkCoords, int32> ChunkDataIndexCoords;
	TMap<int32, FChunkCoords> ChunkBuildingDataIndexCoords;

	
	float StreamingUpdateElapse = 0.f;

	bool bStreamingStart = false;

	bool bHasLastStreamingCenter = false;

	FChunkCoords LastStreamingCenter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	AChunkActor* GetChunk(FVector Location);

	void ClearAll();
	void ClearBuildOnly();

	bool UpgradeBuilding(int32 ID, FString Name, UStaticMesh* NewMesh, UDestructibleMesh* NewDestructibleMesh, int32 NewDurability, int32 NewMaxDurability, EnvironmentsType NewType);

	UFUNCTION(CallInEditor)
		void DestroyBuilding_Editor()
	{
		DestroyBuilding(TargetID);
	}

	UFUNCTION(CallInEditor)
		void EnvironmentActorsToHISM_Editor()
	{
		EnvironmentActorsToHISM();
	}
	UFUNCTION(CallInEditor)
		void HISMToEnvironmentActors_Editor()
	{
		HISMToEnvironmentActors();
	}
	void EnvironmentActorsToHISM();

	UFUNCTION(CallInEditor)
		void EnvironmentActorsClearAll_Editor()
	{
		EnvironmentActorsClearAll();
	}
	UFUNCTION(CallInEditor)
		void ResetChunkStreamingStats_Editor();

	UFUNCTION(CallInEditor, Category = "Benchmark")
		void PrintMemoryStats_Editor();
	void EnvironmentActorsClearAll();
	void HISMToEnvironmentActors();
	void DestroyBuilding(int32 ID);
	void DestroyResource(int32 ID);
	void DisconnectBuilding(int32 ID);
	void RemoveOnlyBuilding(int32 ID);

	bool AddSocket(int32 ID, int32 ConnectID, FName SocketName, FName ConnectSocketName);
	bool AddHISMAttachBuildActor(int32 ID, FName SocketName, ABuildable* Buildable);

	bool CheckSocket(int32 ID, FName SocketName);

	bool RemoveHISMAttachBuildActor(int32 ID, FName SocketName);


	FChunkCoords GetChunkCoordsLocation(const FVector& Location);

	bool GetBuildingTransform(int32 ID, FTransform& Transform);
	bool GetResourceTransform(int32 ID, FTransform& Transform);

	void MarkChunkDirty(const FChunkCoords& Coords);
	void MarkGraphChunkDirty(const FChunkCoords& Coords);
	void MarkChunkDirtyBuildingID(int32 ID);
	void MarkGraphChunkDirtyBuildingID(int32 ID);
	void MarkChunkDirtyResourceID(int32 ID);

	void MarkChunkDirtyLocation(const FVector& Location);

	bool DamageBuilding(int32 ID, int32 Damage);
	bool DamageResource(int32 ID, int32 Damage);
	
	int32 AddBuildingInstanceID(UStaticMesh* Mesh,const FTransform& Transform,FString Name,UDestructibleMesh* DestructibleMesh,int32 Durability,
		int32 MaxDurability,EnvironmentsType Type,EBuildType BType, bool IsAnchor, int32 ID = INDEX_NONE);

	int32 AddResourceInstanceID(UStaticMesh* Mesh,const FTransform& Transform,int32 Hp,EnvironmentsType Type, FString Name,int32 ID = INDEX_NONE);
	
	int32 AddBuildingInstanceDefinitionName(FString BuildDefinitionName,const FTransform& Transform,int32 ID = INDEX_NONE,int32 Durability = INDEX_NONE);

	int32 AddResourceInstanceDefinitionName(FString ResourceDefinitionName,const FTransform& Transform, int32 ID = INDEX_NONE, int32 Hp = INDEX_NONE);
	bool UpgradeBuildingDefinitionName(int32 ID,FString BuildDefinitionName);
	
	void SaveWorld();
	void SaveWorldFull();
	void SaveWorldDirty();
	void LoadWorld();
	
	

	FBuildingNode& GetBuildMapNode(int32 ID) { return BuildingMap[ID]; }
	FResourceNode& GetResourceMapNode(int32 ID) { return ResourceMap[ID];}

	bool FindBuildMapNode(int32 ID) { if (BuildingMap.Find(ID)) return true; else return false; }
	bool FindResourceMapNode(int32 ID) { if (ResourceMap.Find(ID)) return true; else return false; }

	bool BuildChunkSaveData(FChunkCoords& Coords, struct FChunkSaveData& OutData);

	bool HasHISMAttachBuildActors(int32 ID, FName SocketName) {
		FAttachBuildKey Key(ID, SocketName);
		return HISMAttachBuildActors.Contains(Key);}


	void StartChunkStreaming();
	UFUNCTION(CallInEditor)
	void StartChunkStreaming_Editor();

	void StopChunkStreaming();

	UFUNCTION(CallInEditor)
	void StopChunkStreaming_Editor();

	void ForceUpdateChunkStreaming();
	UFUNCTION(CallInEditor)
	void ForceUpdateChunkStreaming_Editor();

	UFUNCTION(CallInEditor)
		void PrintChunkStreamingStats_Editor();


	
	const FChunkStreamingStats& GetChunkStreamingStats() {return ChunkStreamingStats;}
	
	AChunkActor* GetChunkCoords(const FChunkCoords& Coords);
	
	void UpdateChunkStreamingAroundLocation(const FVector& Location, bool bForce);

	bool LoadRuntimeSaveCache();

	void RebuildChunkDataIndex();

	bool SaveRuntimeSaveCacheSlot();

	FChunkSaveData* FindSaveCacheChunkData(const FChunkCoords& Coords);

	void UpsertSaveCacheChunkData(const FChunkCoords& Coords, const FChunkSaveData& ChunkSaveData);


	void RemoveSaveCacheChunkData(const FChunkCoords& Coords);

	void RecalculateStreamingStats();

	void LogChunkStreamingStats(const FString& String);
	void AppendChunkStreamingCSV(const FString& String);

	TSet<FChunkCoords> BuildWantChunkSet(const FChunkCoords& CenterCoords);
	TSet<FChunkCoords> BuildConnectedWantedChunkSet(const TSet<FChunkCoords>& WantedChunks);
	
	void FlushDirtyChunksSaveCache();

	bool LoadChunkRuntime(const FChunkCoords& Coords);
	bool UnloadChunkRuntime(const FChunkCoords& Coords);

	//bool LoadChunkRuntime(const FChunkCoords& Coords, TSet<int32>& LoadChunkBuildingIDs, TSet<FChunkCoords>& ConnectLoadCoords);
	//bool UnloadChunkRuntime(const FChunkCoords& Coords, TSet<int32>& UnLoadChunkBuildingIDs, TSet<FChunkCoords>& ConnectUnLoadCoords);

	bool SpawnAttachBuildableSaveData(const FAttachBuildableSaveData& SaveData);

	FString ChunkCoordsString(const FChunkCoords& Coords) { return FString::Printf(TEXT("(%d, %d)"), Coords.X, Coords.Y); }

	bool SaveLoadChunkCache(const FChunkCoords& Coords);

	void RemoveBuildingIDSaveCachedData(int32 RemoveBuildingID);
	void RemoveResourceIDSaveCachedData(int32 RemoveResourceID);
	/*
	
	
	*/

	AChunkActor* GetOrLoadChunkCoords(const FChunkCoords& Coords, bool CreateMissing = true);

	AChunkActor* FindLoadChunk(const FChunkCoords& Coords);
	AChunkActor* SpawnChunkActorOnly(const FChunkCoords& Coords);

	float GetChunkSize() { return ChunkSize; }
	UFUNCTION(CallInEditor, Category = "Chunk|Debug")
		void DrawChunkGrid_Editor();
	private:

		// Chunk Debug Draw
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			int32 DebugChunkGridRadius = 3;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			FVector DebugChunkLocation;
		// 3이면 중심 기준 7x7 청크 표시

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			float DebugChunkGridZOffset = 50.f;
		// 선이 바닥에 묻히면 100~200 정도로 올리면 됨

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			float DebugChunkGridThickness = 4.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			bool bDebugDrawChunkLabel = true;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			bool bDebugHighlightLoadedChunks = true;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			FColor DebugChunkGridColor = FColor::Cyan;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			FColor DebugLoadedChunkColor = FColor::Green;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Debug", meta = (AllowPrivateAccess = "true"))
			FColor DebugCenterChunkColor = FColor::Yellow;

	

		UFUNCTION(CallInEditor, Category = "Chunk|Debug")
			void ClearChunkGrid_Editor();



		UFUNCTION(CallInEditor)
			void Benchmark_SaveWorldFull_Editor();

		UFUNCTION(CallInEditor)
			void Benchmark_DirtySave1_Editor();

		UFUNCTION(CallInEditor)
			void Benchmark_DirtySave5_Editor();

		void RunDirtySaveBenchmark(int32 DirtyChunkTargetCount);
		bool MakeDirtyChunksForBenchmark(int32 TargetChunkCount);
};
