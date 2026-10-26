// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable.h"
#include "Components/BoxComponent.h"
#include "Engine/CollisionProfile.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../PPGameStateBase.h"
#include "../PPGameInstance.h"
#include "BuildManagerComponent.h"
// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	m_DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("BuildDestructibleMesh"));
	m_DestructibleMesh->SetVisibility(false);
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildStaticMesh"));
	SetRootComponent(m_StaticMesh);
	
	m_DestructibleMesh->SetupAttachment(RootComponent);

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	Mtrl(TEXT("Material'/Game/Materials/BuildO.BuildO'"));
	if (Mtrl.Succeeded())
		CheckSucceededMaterial = Mtrl.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	Mtrl2(TEXT("Material'/Game/Materials/BuildX.BuildX'"));
	if (Mtrl2.Succeeded())
		CheckFailMaterial = Mtrl2.Object;
	HitAmount = 0;
	BuildCheck = false;
	PreBuildCheck = true;
	ActorSize = CollisionVolume->GetScaledBoxExtent();
	m_DestroyTimeMax = 2.0f;
	Destruct = false;
	m_DestroyTime = 0.f;
	FirstCheck = false;
	GroundBuild = false;
	OnlyGrundBuild = false;
	Anchor = false;
	DoOnce = true;
}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();
	
	ActorSize = CollisionVolume->GetScaledBoxExtent();
	CollisionVolume->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	m_StaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	m_DestructibleMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	

	if (Destruct)
	{
		m_DestroyTime += DeltaTime;

		if (m_DestroyTimeMax <= m_DestroyTime)
		{
			Destroy();
			Destruct = false;
		}
	}
	
	
}

void ABuildable::Build()
{
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	params.bFindInitialOverlaps = true;

	TArray<FHitResult>	resultArray;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel13,
		FCollisionShape::MakeBox(ActorSize + 20.f),
		params);

	AChunkManager* ChunkManager = Cast<APPGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GetChunkManager();

	int32 ID;
	//AddBuildingInstanceDefinitionName를 안쓰는 이유는 이미 다 데이터 셋해놨는데 또 인포에서 불러와서 찾는게 비효율적이라 생각해서 안함
	ID = ChunkManager->AddBuildingInstanceID(m_StaticMesh->GetStaticMesh(), GetTransform(), BuildName, m_DestructibleMesh->GetDestructibleMesh(), Durability, DurabilityMax,
		m_MaterialType, m_BuildType, Anchor);
	if (ID == INDEX_NONE)
	{
		
		Destroy();
		return;
	}
	
	TSet<int> CheckedID;
	if (Hit)
	{
		
		for (auto& result : resultArray) {

			UHierarchicalInstancedStaticMeshComponent* HitHISM = Cast<UHierarchicalInstancedStaticMeshComponent>(result.Component.Get());
			AChunkActor* HitChunk = Cast<AChunkActor>(result.GetActor());
			if (IsValid(HitHISM) && IsValid(HitChunk)) {
				UStaticMesh* HitMesh = HitHISM->GetStaticMesh();

				// ★ 핵심: LineTrace가 알려준 인덱스(Item)를 가져옵니다.
				int32 HitIndex = result.Item;
				int32 TargetID = HitChunk->GetMeshID(HitHISM, HitIndex);
				if (TargetID != -1 && TargetID < 1000000)
				{
				
					if (CheckedID.Contains(TargetID))
						continue;
					if (!ChunkManager->FindBuildMapNode(TargetID))
						continue;
					CheckedID.Add(TargetID);
					FBuildingNode TargetNode = ChunkManager->GetBuildMapNode(TargetID);
					FTransform InstanceTransform;
					HitHISM->GetInstanceTransform(TargetNode.InstanceIndex, InstanceTransform, true);
					FVector NodeLocation = InstanceTransform.GetLocation();

					if (AllowedTargetTypes.Contains(TargetNode.BuildType))
					{
						TArray<class UStaticMeshSocket*>& Sockets = HitMesh->Sockets;

						UStaticMeshSocket* FindSocket = nullptr;
						
						
						for (UStaticMeshSocket* Socket : Sockets)
						{
							
							FVector SocketWorldLocation = InstanceTransform.TransformPosition(Socket->RelativeLocation);
							
							if (FVector::Dist(GetActorLocation(), SocketWorldLocation) <= 3.f)
							{
								FindSocket = Socket;
								break;
							}
							if (FindSocket)break;
						}
						/*
						if (FindSocket)
							PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("FindSocket")));
						else
							PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("!!!!!!FindSocket")));
						*/
						if (!FindSocket)
							continue;
						UStaticMeshSocket* FindMySocket = nullptr;
						for (UStaticMeshSocket* Socket : m_StaticMesh->GetStaticMesh()->Sockets) {
							FVector SocketWorldLocation = m_StaticMesh->GetSocketLocation(Socket->SocketName);
							if (FVector::Dist(NodeLocation, SocketWorldLocation) <= 3.f)
							{
								FindMySocket = Socket;
							}
						}
						
						/*
						if (!FindMySocket)
							PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("!FindMySocket")));
							*/
						
						if (FindSocket && FindMySocket) {
							//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("FindSocket && FindMySocket")));
							bool bAddSocket = ChunkManager->AddSocket(ID, TargetID, FindMySocket->SocketName, FindSocket->SocketName);
							ensureMsgf(bAddSocket, TEXT("AddSocket"));
						}
					}
				}
			}
		}
	}
	if (!GroundBuild)
	{
		if (ChunkManager->FindBuildMapNode(ID))
		{
			FBuildingNode Node = ChunkManager->GetBuildMapNode(ID);
			if (Node.ConnectedBuildingIDs.Num() == 0)
			{
				ChunkManager->RemoveOnlyBuilding(ID);
				Destroy();
				return;
			}
		}
	}

	Destroy();
}

void ABuildable::BuildChecking()
{
	//
	if (PreBuildCheck != BuildCheck || !FirstCheck) {
		FirstCheck = true;
		PreBuildCheck = BuildCheck;
		if (BuildCheck) {

			m_StaticMesh->SetMaterial(0, CheckSucceededMaterial);
		}
		else
		{
			m_StaticMesh->SetMaterial(0, CheckFailMaterial);
		}
	}
}
/*
void ABuildable::BuildChecking()
{
	
	if (BuildCheck) {
		m_StaticMesh->SetMaterial(0, CheckSucceededMaterial);
	}
	else
	{
		
		m_StaticMesh->SetMaterial(0, CheckFailMaterial);
	}
}
*/

void ABuildable::SetMesh(UDestructibleMesh* _Destructiblemesh, UStaticMesh* _Staticmesh)
{
	m_DestructibleMesh->SetDestructibleMesh(_Destructiblemesh);
	m_StaticMesh->SetStaticMesh(_Staticmesh);

	for (int32 i = 0; i < m_StaticMesh->GetMaterials().Num(); i++)
		BaseMaterials.Add(m_StaticMesh->GetMaterials()[i]);


	for (int32 i = 0; i < m_StaticMesh->GetMaterials().Num(); i++)
	{
		m_StaticMesh->SetMaterial(i, nullptr);
	}

}

void ABuildable::BuildDestroy() {
	if (!DoOnce)return;
	AChunkManager* ChunkManager = Cast<APPGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GetChunkManager();
	if (ChunkManager && AttachParentBuildingID != INDEX_NONE && !AttachParentSocketName.IsNone())
	{
		ChunkManager->RemoveHISMAttachBuildActor(AttachParentBuildingID, AttachParentSocketName);
	}
	DoOnce = false;
	m_DestructibleMesh->SetVisibility(true);
	m_StaticMesh->SetVisibility(false);
	m_DestructibleMesh->ApplyDamage(100.f, GetActorLocation(), FVector(0.f, 0.f, -1.f) , 100.f);
	Destruct = true;
	CollisionVolume->SetCollisionProfileName("NoCollision");
	m_StaticMesh->SetCollisionProfileName("NoCollision");
	/*
	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		Ceilings[i]->Pillardelete(this);
	}

	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		Pillars[i]->Ceilingsdelete(this);
	}*/

}

float ABuildable::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	Durability -= (int32)Damage;
	if (Durability <= 0)
		BuildDestroy();
	else {
		if (AttachParentBuildingID != INDEX_NONE) {
			AChunkManager* ChunkManager = Cast<APPGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GetChunkManager();
			if (ChunkManager)
			{
				ChunkManager->MarkChunkDirtyBuildingID(AttachParentBuildingID);
			}
		}
	}
	return Damage;
}

void ABuildable::Pillardelete(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		if (Pillars[i] == _Pillar)
		{
			Pillars.RemoveAt(i);
			i--;
		}
	}

	if (Pillars.Num() <=1)
	{

		BuildDestroy();

	}
}

void ABuildable::Ceilingsdelete(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		if (Ceilings[i] == _Pillar)
		{
			Ceilings.RemoveAt(i);
			i--;
		}
	}
}

void ABuildable::AddPillar(ABuildable* _Pillar)
{
	for (int32 i = 0; i < Pillars.Num(); i++)
	{
		if (Pillars[i] == _Pillar)
			return;
	}
	Pillars.Add(_Pillar);
}

void ABuildable::AddCeilings(ABuildable* _Ceiling)
{
	for (int32 i = 0; i < Ceilings.Num(); i++)
	{
		if (Ceilings[i] == _Ceiling)
			return;
	}
	Ceilings.Add(_Ceiling);
}

void ABuildable::SetBuildDefinition(const FBuildDefinitionInfo* Info,FString Name,UDestructibleMesh* DestructibleMesh, UStaticMesh* StaticMesh)
{
	BuildDefinitionInfo = Info;
	SetBuildType(Info->BuildType);
	SetMesh(DestructibleMesh, StaticMesh);
	SetDurability(Info->Durability);
	SetMaxDurability(Info->MaxDurability);
	SetMaterialType(Info->MaterialType);
	SetBuildName(Name);
	SetGroundBuild(Info->GroundBuild);
	SetOnlyGrundBuild(Info->GroundOnly);
	SetAnchor(Info->Anchor);
	SetAllowedTargetTypes(Info->AllowedTargetTypes);
}

void ABuildable::SetAttachParentInfo(int32 ParentID, FName SocketName)
{
	AttachParentBuildingID = ParentID;
	AttachParentSocketName = SocketName;
}

void ABuildable::ApplyMaterialFromBaseMaterials()
{
	if (BaseMaterials.Num() > 0)
	{
		for (int32 i = 0; i < BaseMaterials.Num(); i++)
		{
			m_StaticMesh->SetMaterial(i, BaseMaterials[i]);
		}
	}

	m_StaticMesh->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionVolume->SetCollisionProfileName(TEXT("Buildable"));
}
