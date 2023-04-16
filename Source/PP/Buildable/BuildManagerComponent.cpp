// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildManagerComponent.h"
#include "Buildable.h"

#include "Kismet/KismetMathLibrary.h"

#include "../Character/Player/PlayerCharacter.h"
// Sets default values for this component's properties
UBuildManagerComponent::UBuildManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


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
	/*
	FVector DirectionVector = PlayerCamera->GetForwardVector() * BuildDistance;
	DirectionVector += GetOwner()->GetActorLocation();
	return FVector(FMath::GridSnap(DirectionVector.X, GridSize), FMath::GridSnap(DirectionVector.Y, GridSize),FloorHeight);
	*/

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), result.ImpactNormal.Z, result.ImpactNormal.Y, result.ImpactNormal.Z));

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

	if (IsBuilding)
	{
		//PrintViewport(1.f, FColor::Red, "Ok");
		FVector Location = GetMousePickingLocation();
		//FVector GridLocation = GetGridLocation(Location);
		//FRotator Rotation = GetNextBuildRotation();

		//DrawDebugBox(GetWorld(), Location, FVector(100, 100, 100), FColor::Red, false, 0, 0, 10);
		if (CurrentBuild == nullptr)
		{
			FActorSpawnParameters	param1;
			param1.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			CurrentBuild = GetWorld()->SpawnActor<ABuildable>(BuildiObject[(int32)m_BuildType]->GetClass(), Location, FRotator::ZeroRotator, param1);
			CurrentBuild->SetBuildType(m_BuildType);
			CurrentBuild->SetMesh(DestructibleMesh, StaticMesh);
			CurrentBuild->SetDurability(Durability);
			CurrentBuild->SetMaxDurability(Durability);
			CurrentBuild->SetMaterialType(MaterialType);
			
		}
		else{
			CurrentBuild->BuildChecking(Location);
			//
		}
		//DrawDebugBox(GetWorld(), Location, FVector(100, 100, 100), Rotation.Quaternion(), FColor::Red, false, 0, 0, 10);
	}
	else if (CurrentBuild != nullptr)
	{
		CurrentBuild->Destroy();
		CurrentBuild = nullptr;
		//PrintViewport(1.f, FColor::Red, "Error");
	}
	// ...
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
void UBuildManagerComponent::SetPlayer(APlayerCharacter* _Player) {
	m_Player = _Player;
}
