// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildManagerComponent.h"
#include "Buildable.h"
#include "../PPGameInstance.h"
#include "../PPGameStateBase.h"
#include "../Chunk/ChunkActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DestructibleMesh.h"
#include "../AssetManager/BuildAssetSubsystem.h"
#include "../Character/Player/PlayerCharacter.h"
// Sets default values for this component's properties
UBuildManagerComponent::UBuildManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*
	static ConstructorHelpers::FClassFinder<ABuildable>	Foundationbuild(TEXT("Blueprint'/Game/Buildable/BpFoundationBuildable.BpFoundationBuildable_C'"));

	if (Foundationbuild.Succeeded())
		BuildiObject.Add(Foundationbuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpWallBuild(TEXT("Blueprint'/Game/Buildable/BpWallBuildable.BpWallBuildable_C'"));

	if (BpWallBuild.Succeeded())
		BuildiObject.Add(BpWallBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpCeilingBuild(TEXT("Blueprint'/Game/Buildable/BpCeilingBuildable.BpCeilingBuildable_C'"));

	if (BpCeilingBuild.Succeeded())
		BuildiObject.Add(BpCeilingBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpFrameDoorBuild(TEXT("Blueprint'/Game/Buildable/BpDoorFrameBuildable.BpDoorFrameBuildable_C'"));

	if (BpFrameDoorBuild.Succeeded())
		BuildiObject.Add(BpFrameDoorBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpFrameWindowBuild(TEXT("Blueprint'/Game/Buildable/BpWindowFrameBuildable.BpWindowFrameBuildable_C'"));

	if (BpFrameWindowBuild.Succeeded())
		BuildiObject.Add(BpFrameWindowBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpRampBuild(TEXT("Blueprint'/Game/Buildable/BpRampBuildable.BpRampBuildable_C'"));

	if (BpRampBuild.Succeeded())
		BuildiObject.Add(BpRampBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpStairsBuild(TEXT("Blueprint'/Game/Buildable/BpStairsBuildable.BpStairsBuildable_C'"));

	if (BpStairsBuild.Succeeded())
		BuildiObject.Add(BpStairsBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpFenceBuild(TEXT("Blueprint'/Game/Buildable/BpFenceBuildable.BpFenceBuildable_C'"));

	if (BpFenceBuild.Succeeded())
		BuildiObject.Add(BpFenceBuild.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpDoorBuildable(TEXT("Blueprint'/Game/Buildable/BpDoorBuildable.BpDoorBuildable_C'"));

	if (BpDoorBuildable.Succeeded())
		BuildiObject.Add(BpDoorBuildable.Class.GetDefaultObject());

	static ConstructorHelpers::FClassFinder<ABuildable>	BpWindowBuildable(TEXT("Blueprint'/Game/Buildable/BpWindowBuildable.BpWindowBuildable_C'"));

	if (BpWindowBuildable.Succeeded())
		BuildiObject.Add(BpWindowBuildable.Class.GetDefaultObject());
		*/
	bOverlapCheck = true;
	/*
	BuildableTypeSocketNames.SetNum(static_cast<int32>(EBuildType::None));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ceiling)].Add(TEXT("UpCeilingSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ceiling)].Add(TEXT("UpCeilingSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ceiling)].Add(TEXT("UpCeilingSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ceiling)].Add(TEXT("UpCeilingSocket4"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::DoorFrame)].Add(TEXT("UpWallSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::DoorFrame)].Add(TEXT("UpWallSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::DoorFrame)].Add(TEXT("UpWallSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::DoorFrame)].Add(TEXT("UpWallSocket4"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Foundation)].Add(TEXT("FoundationSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Foundation)].Add(TEXT("FoundationSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Foundation)].Add(TEXT("FoundationSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Foundation)].Add(TEXT("FoundationSocket4"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ramp)].Add(TEXT("FoundationSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ramp)].Add(TEXT("FoundationSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ramp)].Add(TEXT("FoundationSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Ramp)].Add(TEXT("FoundationSocket4"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Stairs)].Add(TEXT("StairsSocket"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Wall)].Add(TEXT("UpWallSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Wall)].Add(TEXT("UpWallSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Wall)].Add(TEXT("UpWallSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Wall)].Add(TEXT("UpWallSocket4"));


	BuildableTypeSocketNames[static_cast<int32>(EBuildType::WindowFrame)].Add(TEXT("UpWallSocket1"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::WindowFrame)].Add(TEXT("UpWallSocket2"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::WindowFrame)].Add(TEXT("UpWallSocket3"));
	BuildableTypeSocketNames[static_cast<int32>(EBuildType::WindowFrame)].Add(TEXT("UpWallSocket4"));


	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Window)].Add(TEXT("WindowSocket"));

	BuildableTypeSocketNames[static_cast<int32>(EBuildType::Door)].Add(TEXT("DoorSocket"));
	*/
	// ...
}


// Called when the game starts
void UBuildManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GridSize = FVector(100.f, 100.f, 100.f);
	BuildDistance = 500.f;
	FloorHeight = 130.f;
	IsBuilding = false;

	PlayerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (PlayerCamera == nullptr) {
		//PrintViewport(1.f, FColor::Red, "PlayerCamera Error");
	}
	PlayerArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	if (PlayerArm == nullptr)
	{
		//PrintViewport(1.f, FColor::Red, "PlayerArm Error");
	}
}

FVector UBuildManagerComponent::GetMousePickingLocation()
{
	
	FCollisionQueryParams	params(NAME_None, false, GetOwner());
	params.bReturnPhysicalMaterial = true;

	FHitResult	result;
	/*
	FVector WolodLocation;
	FVector WolodDirection;
	
	if (GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WolodLocation, WolodDirection))
	{
		float ArmLength = PlayerArm->TargetArmLength *10.f;
		//FVector End = WolodLocation + (WolodDirection * ArmLength);
		
		//FVector Location = m_ArrowSpawnLocation->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(m_Player->GetControl_Rotation()) * 500.f);
			//return;
		
		bool Hit = GetWorld()->LineTraceSingleByChannel(result, PlayerCamera->GetComponentLocation(), End,
			ECollisionChannel::ECC_GameTraceChannel1, params);
			
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), result.ImpactPoint.X, result.ImpactPoint.Y, result.ImpactPoint.Z));
		
		
		
		//DrawDebugBox(GetWorld(), result.ImpactPoint.GetSignVector() , FVector(10, 10, 10), FRotator(0, 1.f, 0), FColor::Red, false, 0, 0, 10);
		return result.ImpactPoint;
	}
	else
	{
	return FVector(0, 0, 0);
	}
	*/

	FVector End = PlayerCamera->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(PlayerCamera->GetComponentRotation()) * 1000);

	bool Hit = GetWorld()->LineTraceSingleByChannel(result, PlayerCamera->GetComponentLocation(), End,
		ECollisionChannel::ECC_GameTraceChannel1, params);
	
	

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), result.ImpactNormal.Z, result.ImpactNormal.Y, result.ImpactNormal.Z));
	
	return result.ImpactPoint;
}
FVector UBuildManagerComponent::GetGridLocation(FVector MouseLocation)
{
	FVector GridLocation = MouseLocation / GridSize;
	int x = GridLocation.X;
	int y = GridLocation.Y;
	int z = GridLocation.Z;
	GridLocation = FVector(x, y, z);
	GridLocation = GridLocation * GridSize;
	//GridLocation = GridLocation + (GridSize * FVector(-1.f, -1.f, 0));
	return GridLocation;
}
FRotator UBuildManagerComponent::GetNextBuildRotation()
{
	FVector(148.f, 148.f, 122.f);

	FRotator Rotation = PlayerCamera->GetComponentRotation();
	return FRotator(0,FMath::GridSnap(Rotation.Yaw,90.f),0);
}

// Called every frame
void UBuildManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	 Building();
}

UStaticMeshSocket* UBuildManagerComponent::FindClosestSocket(const FBuildingNode& Node, FVector Location)
{
	
	if (!Node.Mesh || !Node.OwnerChunk || !BuildDefinitionInfo || !CurrentBuild) return nullptr;

	if (CurrentBuild->GetOnlyGrundBuild() && CurrentBuild->GetBuildType() != Node.BuildType)
		return nullptr;
		
	
	FTransform InstanceTransform;
	UHierarchicalInstancedStaticMeshComponent* HISM = Node.OwnerChunk->FindHISM(Node.Mesh, EObType::Building);

	if (!HISM || !(0 <= Node.InstanceIndex && Node.InstanceIndex < HISM->GetInstanceCount())) return nullptr;

	HISM->GetInstanceTransform(Node.InstanceIndex, InstanceTransform, true);
	
	TArray<class UStaticMeshSocket*> Sockets = Node.Mesh->Sockets;

	UStaticMeshSocket* ClosestSocket = nullptr;
	float Min = MAX_FLT;


	for (UStaticMeshSocket* Socket : Sockets)
	{
		if (!Socket) continue;
		
		if (!BuildDefinitionInfo->AllowedTargetSockets.Contains(Socket->SocketName))
			continue;
		
		FVector SocketWorldLocation = InstanceTransform.TransformPosition(Socket->RelativeLocation);

		float Dist = FVector::DistSquared(Location, SocketWorldLocation);

		if (Dist < Min)
		{
			Min = Dist;
			ClosestSocket = Socket;
		}
	}
if (ClosestSocket) {
		
		FVector SocketLocation = InstanceTransform.TransformPosition(ClosestSocket->RelativeLocation);
		FQuat WorldRotation = InstanceTransform.TransformRotation(ClosestSocket->RelativeRotation.Quaternion());
		float dis = FVector::Dist(Location, InstanceTransform.GetLocation());
		float LodeAndSocketDist = FVector::Dist(InstanceTransform.GetLocation(), SocketLocation);
		if (dis < LodeAndSocketDist && dis < 100.f) {
			return nullptr;
		}
		CurrentBuild->SetActorLocation(InstanceTransform.TransformPosition(ClosestSocket->RelativeLocation));
		CurrentBuild->SetActorRotation(WorldRotation);
		
	}


	return ClosestSocket;
}
/*
bool UBuildManagerComponent::CheckBuildableTypeSocketNames(EBuildType Type, FName SocketName)
{

	return BuildableTypeSocketNames[static_cast<int32>(Type)].Contains(SocketName);
}
*/

void UBuildManagerComponent::Building()
{

	if (IsBuilding && BuildDefinitionInfo)
	{
		if (CurrentBuild == nullptr)
		{
			if (!bPreviewAssetsReady || !CurrentAssets.IsValid())
				return;

	
			FActorSpawnParameters	param1;
			param1.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
			CurrentBuild = GetWorld()->SpawnActor<ABuildable>(CurrentAssets.BuildClass, FVector::ZeroVector, FRotator::ZeroRotator, param1);
			if (!CurrentBuild)
				return;
			CurrentBuild->SetActorScale3D(FVector(1.001, 1.001, 1.001));
			CurrentBuild->SetBuildDefinition(BuildDefinitionInfo, BuildDefinitionName, CurrentAssets.DestructibleMesh, CurrentAssets.StaticMesh);

		}
		FCollisionQueryParams	params(NAME_None, false, GetOwner());
		params.bReturnPhysicalMaterial = true;
	
		FHitResult	result;

		FVector End = PlayerCamera->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(PlayerCamera->GetComponentRotation()) * 1000);
	
		bool Hit = GetWorld()->LineTraceSingleByChannel(result, PlayerCamera->GetComponentLocation(), End,
			ECollisionChannel::ECC_GameTraceChannel1, params);

	
		FVector Location = result.ImpactPoint;
		CurrentBuild->SetActorLocation(Location);
		UInstancedStaticMeshComponent* HitHISM = Cast<UInstancedStaticMeshComponent>(result.Component.Get());
		AChunkActor* HitChunk = Cast<AChunkActor>(result.GetActor());
		
	
		bool GroundCheck = true;
		if (IsValid(HitHISM) && IsValid(HitChunk)) {
			UStaticMesh* HitMesh = HitHISM->GetStaticMesh();
			// ★ 핵심: LineTrace가 알려준 인덱스(Item)를 가져옵니다.
			int32 HitIndex = result.Item;
			int32 TargetID = HitChunk->GetMeshID(HitHISM, HitIndex);
			
			if (TargetID != -1 && TargetID < 1000000)
			{
				AChunkManager* ChunkManager= Cast<APPGameStateBase>(UGameplayStatics::GetGameState(GetWorld()))->GetChunkManager();
				if (!ChunkManager->FindBuildMapNode(TargetID))
					return;
				FBuildingNode TargetNode = ChunkManager->GetBuildMapNode(TargetID);
				
				UStaticMeshSocket* Socket = FindClosestSocket(TargetNode, result.ImpactPoint);
				bOverlapCheck = OverlapCheckCanBuild();
				if (Socket) {
					
					bOverlapCheck = bOverlapCheck && !TargetNode.SocketMap.Contains(Socket->SocketName) && !ChunkManager->HasHISMAttachBuildActors(TargetID, Socket->SocketName);
				
				}
			}
		}
		else {
			bOverlapCheck = OverlapCheckCanBuild();
			if (!CurrentBuild->GetGroundBuild())
				GroundCheck = false;
		}
		
		if (!bOverlapCheck || !GroundCheck)
		{
			if (CurrentBuild->GetBuildCheck() == true)
				CurrentBuild->SetBuildCheck(false);
		}
		else {

			if (CurrentBuild->GetBuildCheck() == false)
				CurrentBuild->SetBuildCheck(true);

		}
			CurrentBuild->BuildChecking();
		
	}

	else if (CurrentBuild != nullptr)
	{
		CurrentBuild->Destroy();
		CurrentBuild = nullptr;
		//PrintViewport(1.f, FColor::Red, "Error");
	}
	// ...
}
bool UBuildManagerComponent::OverlapCheckCanBuild()
{
	FCollisionQueryParams Params(NAME_None, false, CurrentBuild);
	bool bIsOverlapping = GetWorld()->OverlapBlockingTestByChannel(
		CurrentBuild->GetActorLocation(),
		CurrentBuild->GetActorRotation().Quaternion(),
		ECC_GameTraceChannel13,
		FCollisionShape::MakeBox(CurrentBuild->GetActorSize()*0.9),
		Params);
	
	return !bIsOverlapping;
}

void UBuildManagerComponent::ToggleBuildMode()
{
	IsBuilding = !IsBuilding;
	//PrintViewport(1.f, FColor::Red, "ToggleBuildMode()");
}

bool UBuildManagerComponent::RequestBuild()
{
	//PrintViewport(1.f, FColor::Red, "RequestBuild()");

	if (!IsBuilding || CurrentBuild == nullptr) {
		return false;
	}
	if (CurrentBuild->GetBuildCheck())
	{
		CurrentBuild->SetActorScale3D(FVector(1, 1, 1));
		CurrentBuild->Build();
		
		CurrentBuild = nullptr;
		return true;
	}
	return false;
}

ABuildable* UBuildManagerComponent::GetCurrentBuild() {
	if (CurrentBuild)
	{
		return CurrentBuild;
	}
	else {
		return nullptr;
	}

}
void UBuildManagerComponent::SetBuildData(FString Name)
{
	BuildDefinitionName = Name;
	BuildDefinitionInfo = nullptr;
	CurrentAssets = FBuildAssets();
	bPreviewAssetsReady = false;
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	if (Inst)
		BuildDefinitionInfo = Inst->FindBuildInfo(BuildDefinitionName);

	if (!BuildDefinitionInfo)
		return;

	UBuildAssetSubsystem* Resolver =
		GetWorld()->GetGameInstance()->GetSubsystem<UBuildAssetSubsystem>();

	if (!Resolver)
		return;

	Resolver->RequestAsync(
		BuildDefinitionName,
		FBuildAssetsLoad::CreateWeakLambda(
			this,
			[this, RequestedId = BuildDefinitionName](bool bSuccess, FBuildAssets Assets)
			{
				if (!bSuccess)
					return;

				if (BuildDefinitionName != RequestedId)
					return;

				CurrentAssets = Assets;
				bPreviewAssetsReady = true;
			}));
}
void UBuildManagerComponent::BuilingDestroy()
{
	if (!CurrentBuild)
		return;
	CurrentBuild->Destroy();
	CurrentBuild = nullptr;
}

void UBuildManagerComponent::SetPlayer(APlayerCharacter* _Player) {
	m_Player = _Player;
}
