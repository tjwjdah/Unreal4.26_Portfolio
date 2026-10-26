// Fill out your copyright notice in the Description page of Project Settings.


#include "AInstancingBenchmark.h"

#include "../Character/Player/PlayerCharacter.h"
#include "../PPGameInstance.h"
#include "../Buildable/BuildManagerComponent.h"
#include "../Buildable/Buildable.h"
// Sets default values
AAInstancingBenchmark::AAInstancingBenchmark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GridSizeX = 30;
	GridSizeY = 30;
	GridSizeZ = 5;
	Spacing = 300.f;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	ISMComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMComp"));
	ISMComp->SetupAttachment(RootComponent);
	ISMComp->SetCollisionProfileName(TEXT("BlockAll"));
	
	// 2. HISM 컴포넌트 생성
	HISMComp = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISMComp"));
	HISMComp->SetupAttachment(RootComponent);
	HISMComp->SetCollisionProfileName(TEXT("BlockAll"));
	Name = TEXT("WoodWall");
	Chunk = true;
}

// Called when the game starts or when spawned
void AAInstancingBenchmark::BeginPlay()
{
	Super::BeginPlay();
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const UDataTable* Table = GameInst->GetUIItemInfo();
	TArray<FUIItemTableInfo*> List;
	FString context;
	Table->GetAllRows<FUIItemTableInfo>(context, List);
	for (int32 i = 0; i < List.Num(); i++)
	{
		if (List[i]->ItemType == EItemType::BuildItem)
		{
			Names.Add(List[i]->Name);
		}
	}
	ISMComp->SetWorldLocation(FVector::ZeroVector);
	HISMComp->SetWorldLocation(FVector::ZeroVector);

	ChunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
	SetBenchmark();
}

// Called every frame
void AAInstancingBenchmark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAInstancingBenchmark::SetBenchmark()
{
	if (Name == TEXT(""))
		return;
	if (Name == TEXT("Tree"))
	{
		return;
	}
	else if(Name == TEXT("Stone")) {

		return;
	}
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(Name);
	Mesh = Info->Mesh;
	DestructibleMesh = Info->DestructibleMesh;
	BuildType = Info->EBuildType;
	for (int32 i = 0; i < Info->OptionArray.Num(); i++) {
		if (Info->OptionArray[i].OptionType == EItemOption::Durability)
		{
			Durability = Info->OptionArray[i].Option;
			switch (Info->OptionArray[i].Option)
			{
			case 1000:MaterialType = EnvironmentsType::Wood;
				break;
			case 2000: MaterialType = EnvironmentsType::Stone;
				break;
			case 3000: MaterialType = EnvironmentsType::Metal;
				break;
			}
		}
	}
	const FBuildDefinitionInfo* BuildInfo = GameInst->FindBuildInfo(Name);
	ActorSpawn = BuildInfo->BuildableClass.LoadSynchronous();
}

void AAInstancingBenchmark::RunBenchmark()
{
	ClearAll();

	if (!Mesh) {
		return;
	}
	
	ISMComp->SetMobility(EComponentMobility::Movable);
	HISMComp->SetMobility(EComponentMobility::Movable);
	ISMComp->SetStaticMesh(Mesh);
	HISMComp->SetStaticMesh(Mesh);
	if (Chunk) 
		PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Starting Benchmark Mode: Chunk")));
		else
		PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Starting Benchmark Mode: %d"), CurrentMode));
	
	FVector StartLocation = GetActorLocation();
	int32 ParentID=-1;
	int32 MyID=-1;

	const int32 TotalCount = GridSizeX * GridSizeY * GridSizeZ;

	const double StartSeconds = FPlatformTime::Seconds();

	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 z = 0; z < GridSizeZ; z++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing, y * Spacing, z * Spacing);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceID(Mesh, SpawnTransform,TEXT(""), DestructibleMesh, Durability, Durability, MaterialType, BuildType, false);
					if (z == 0)continue;
					ChunkManager->AddSocket(MyID, ParentID,TEXT(""), TEXT(""));
					continue;
				}

				switch (CurrentMode)
				{
				case EInstancingMode::None:
					return;
				case EInstancingMode::Actor:
				{
					if (ActorSpawn) {
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorSpawn, SpawnTransform, SpawnParams);
						ABuildable* Bulid = Cast<ABuildable>(NewActor);
						if (IsValid(Bulid)) {
							Bulid->SetBuildType(BuildType);
							Bulid->SetMesh(DestructibleMesh, Mesh);
							Bulid->SetDurability(Durability);
							Bulid->SetMaxDurability(Durability);
							Bulid->SetMaterialType(MaterialType);
						}
						SpawnActors.Add(NewActor);
					}
				}
				break;
				case EInstancingMode::ISM:
					ISMComp->AddInstance(SpawnTransform);
					break;
				case EInstancingMode::HISM:
					HISMComp->AddInstance(SpawnTransform);
					break;
				}
			}
		}
	}
	const double EndSeconds = FPlatformTime::Seconds();
	const float ElapsedMs = static_cast<float>((EndSeconds - StartSeconds) * 1000.0);

	UE_LOG(LogTemp, Log,
		TEXT("[InstancingBenchmark] Mode=%s Count=%d CreateTime=%.3f ms"),
		Chunk ? TEXT("Chunk-HISM") : *UEnum::GetValueAsString(CurrentMode),
		TotalCount,
		ElapsedMs);
}

void AAInstancingBenchmark::Test()
{

	FVector StartLocation = GetActorLocation();
	int32 ParentID = -1;
	int32 MyID = -1;

	GridSizeY = 25;
	GridSizeX = 25;
	Spacing = ChunkManager->GetChunkSize();;
	const double StartSeconds = FPlatformTime::Seconds();

	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 i = 0; i < 4; i++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing + (i* 400), y * Spacing, 0.f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceDefinitionName(TEXT("WoodFoundation"), SpawnTransform);
					//ChunkManager->AddSocket(MyID, ParentID, TEXT(""), TEXT(""));
					continue;
				}
			}
		}
	}
	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 i = 0; i < 4; i++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing + (i * 400), y * Spacing +400, 0.f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceDefinitionName(TEXT("WoodWall"), SpawnTransform);
					//ChunkManager->AddSocket(MyID, ParentID, TEXT(""), TEXT(""));
					continue;
				}
			}
		}
	}
	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 i = 0; i < 4; i++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing + (i * 400), y * Spacing +800, 0.f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceDefinitionName(TEXT("WoodCeiling"), SpawnTransform);
					//ChunkManager->AddSocket(MyID, ParentID, TEXT(""), TEXT(""));
					continue;
				}
			}
		}
	}
	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 i = 0; i < 1; i++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing + 2500, y * Spacing + 2500, 0.f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceDefinitionName(TEXT("WoodDoorFrame"), SpawnTransform);
					//ChunkManager->AddSocket(MyID, ParentID, TEXT(""), TEXT(""));
					continue;
				}
			}
		}
	}
	for (int32 x = 0; x < GridSizeX; x++) {
		for (int32 y = 0; y < GridSizeY; y++) {
			for (int32 i = 0; i < 1; i++) {
				FVector SpawnLocation = StartLocation + FVector(x * Spacing + (i * 400), y * Spacing + 1600, 0.f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f, 1.0f, 1.0f));

				if (Chunk && ChunkManager) {

					ParentID = MyID;
					MyID = ChunkManager->AddBuildingInstanceDefinitionName(TEXT("WoodStairs"), SpawnTransform);
					//ChunkManager->AddSocket(MyID, ParentID, TEXT(""), TEXT(""));
					continue;
				}
			}
		}
	}
}

void AAInstancingBenchmark::ClearAll()
{

	for(auto* Actor : SpawnActors){
		if (IsValid(Actor))
			Actor->Destroy();
	}
	SpawnActors.Empty();

	if (ChunkManager)ChunkManager->ClearAll();
	if (ISMComp)ISMComp->ClearInstances();
	if (HISMComp)HISMComp->ClearInstances();
	GEngine->ForceGarbageCollection(true);
	return;
	
}

