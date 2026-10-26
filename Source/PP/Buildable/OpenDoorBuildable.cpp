// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorBuildable.h"
#include "DoorBuildable.h"
#include "../Chunk/ChunkManager.h"
#include "../PPGameStateBase.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
#include "../Character/Player/PlayerCharacter.h"
AOpenDoorBuildable::AOpenDoorBuildable() {
	PrimaryActorTick.bCanEverTick = true;
	

	
	//CheckAddSocketType.Add(EBuildType::DoorFrame);
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	DoorAxis = CreateDefaultSubobject<UArrowComponent>(TEXT("DoorAxis"));
	DoorSocket = CreateDefaultSubobject<USceneComponent>(TEXT("DoorSocket"));
	DoorCollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollisionVolume"));
	SetRootComponent(RootSceneComponent);
	DoorCollisionVolume->SetupAttachment(RootComponent);
	DoorAxis->SetupAttachment(RootSceneComponent);
	DoorSocket->SetupAttachment(DoorAxis);
	m_StaticMesh->SetupAttachment(DoorSocket);
	
}

void AOpenDoorBuildable::BeginPlay()
{
	Super::BeginPlay();
	ActorSize = FVector::ZeroVector;
	if (CurveFloat)
	{
		
		FOnTimelineFloat ProgressFunction;


		ProgressFunction.BindUFunction(this, FName("OpenDoor"));


		MyTimeline.AddInterpFloat(CurveFloat, ProgressFunction);


	}
	DoorCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOpenDoorBuildable::DoorBeginOverlap);
	DoorCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AOpenDoorBuildable::DoorEndOverlap);
	m_DelegateCheck = false;
}

void AOpenDoorBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void AOpenDoorBuildable::Build()
{
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	params.bFindInitialOverlaps = true;
	ActorSize = CollisionVolume->GetScaledBoxExtent();
	TArray<FHitResult>	resultArray;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel13,
		FCollisionShape::MakeBox(ActorSize + 20.f),
		params);

	AChunkManager* ChunkManager = Cast<APPGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GetChunkManager();

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
							if (FVector::Dist(GetActorLocation(), SocketWorldLocation) <=3.f)
							{
								FindSocket = Socket;
								break;
							}
							if (FindSocket)break;
						}

						if (!FindSocket)
							continue;

						bool AddObject = ChunkManager->AddHISMAttachBuildActor(TargetID, FindSocket->SocketName,this);
						ensureMsgf(AddObject, TEXT("AddSocket"));
					}
				}
			}
		}
	}

	ApplyMaterialFromBaseMaterials();
}
/*
void AOpenDoorBuildable::BuildChecking(FVector Location)
{
	ResultLocation = Location;
	HitAmount = 2;
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;

	TArray<FHitResult>	resultArray;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		Location, Location, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(ActorSize),
		params);
	//bool Hit = GetWorld()->LineTraceSingleByChannel(result, Location, Location + 100.f,
		//ECollisionChannel::ECC_GameTraceChannel1, params);

	if (Hit)
	{

		for (auto& result : resultArray) {
			if (result.Actor->IsA(ADoorBuildable::StaticClass()))
			{

				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				ResultLocation = Act->GetSceneComponent((int32)EDoorSocket::DoorSocket)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EDoorSocket::DoorSocket)->GetComponentRotation();
				HitAmount--;
				FrameReference = Act;
				
				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
			else
			{
				ResultLocation.Z = Location.Z + ActorSize.Z;
			}
		}
		bool Hit2 = GetWorld()->SweepMultiByChannel(resultArray,
			ResultLocation, ResultLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeBox(ActorSize),
			params);
		for (auto& result : resultArray) {
			if (result.Actor->IsA(AOpenDoorBuildable::StaticClass()))
			{
				//float dis = FVector::Distance(ResultLocation, result.Actor->GetActorLocation());
				if (result.PenetrationDepth > 0.1f)
				{
					HitAmount++;
				}
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), dis));
			}
		}

	}

	if (HitAmount >= 2)
	{
		BuildCheck = false;
	}
	else
	{
		BuildCheck = true;
	}
	//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("%f %f %f"), ActorSize.X, ActorSize.Y, ActorSize.Z));
	SetActorLocation(ResultLocation);
	SetActorRotation(ResultRotator);
	Super::BuildChecking(ResultLocation);

	
}
*/
void AOpenDoorBuildable::BuildDestroy()
{
	Super::BuildDestroy();
}

void AOpenDoorBuildable::OpenDoor(float Value)
{
	DoorAxis->SetRelativeRotation(FRotator(0.f, Value, 0.f));
}

void AOpenDoorBuildable::PlayerOpenDoor()
{
	if (m_Open)
	{
		MyTimeline.Reverse();
		m_Open = false;
		UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

		if (GameInst)
		{
			// 인벤토리에 아이템을 추가한다.
			APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Open"))));
			}
		}
	}
	else
	{
		MyTimeline.Play();
		m_Open = true;
		UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

		if (GameInst)
		{
			// 인벤토리에 아이템을 추가한다.
			APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Close"))));
			}
		}
	}
}

void AOpenDoorBuildable::DoorBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_DelegateCheck = true;
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		// 인벤토리에 아이템을 추가한다.
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (!m_Open)
				GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Open"))));
			else
				GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Close"))));
		}

	}
	Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetDelegate<AOpenDoorBuildable>(this, &AOpenDoorBuildable::PlayerOpenDoor);
}

void AOpenDoorBuildable::DoorEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (!m_DelegateCheck)
		return;
	m_DelegateCheck = false;
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	if (GameInst)
	{

		// 인벤토리에 아이템을 추가한다.
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->DelegateUnBind();
}

