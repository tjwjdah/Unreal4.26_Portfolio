// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingBuildable.h"
#include "WindowBuildable.h"
#include "DoorBuildable.h"
#include "WallBuildable.h"
#include "FoundationBuildable.h"
ACeilingBuildable::ACeilingBuildable() {

	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BuildComponents = CreateDefaultSubobject<USceneComponent>(TEXT("BuildComponents"));
	SceneComponents.Add(BuildComponents);

	USceneComponent* CeilingSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket1"));
	SceneComponents.Add(CeilingSocket1);
	USceneComponent* CeilingSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket2"));
	SceneComponents.Add(CeilingSocket2);
	USceneComponent* CeilingSocket3 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket3"));
	SceneComponents.Add(CeilingSocket3);
	USceneComponent* CeilingSocket4 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket4"));
	SceneComponents.Add(CeilingSocket4);

	USceneComponent* WallSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket1"));
	SceneComponents.Add(WallSocket1);
	USceneComponent* WallSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket2"));
	SceneComponents.Add(WallSocket2);
	USceneComponent* WallSocket3 = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket3"));
	SceneComponents.Add(WallSocket3);
	USceneComponent* WallSocket4 = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket4"));
	SceneComponents.Add(WallSocket4);

	USceneComponent* StairsSocket = CreateDefaultSubobject<USceneComponent>(TEXT("StairsSocket"));
	SceneComponents.Add(StairsSocket);
	USceneComponent* FloorSocket = CreateDefaultSubobject<USceneComponent>(TEXT("FloorSocket"));
	SceneComponents.Add(FloorSocket);
	USceneComponent* TriangleCeilingSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket1"));
	SceneComponents.Add(TriangleCeilingSocket1);
	USceneComponent* TriangleCeilingSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket2"));
	SceneComponents.Add(TriangleCeilingSocket2);
	USceneComponent* TriangleCeilingSocket3 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket3"));
	SceneComponents.Add(TriangleCeilingSocket3);
	USceneComponent* TriangleCeilingSocket4 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket4"));
	SceneComponents.Add(TriangleCeilingSocket4);

	BuildComponents->SetupAttachment(RootComponent);
	for (int32 i = 1; i < SceneComponents.Num(); i++)
	{
		SceneComponents[i]->SetupAttachment(BuildComponents);
	}
	
}

void ACeilingBuildable::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ACeilingBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACeilingBuildable::Build()
{
	Super::Build();
	
	TArray<FHitResult>	resultArray;

	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	params.bFindInitialOverlaps = true;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		GetActorLocation() , GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(FVector(150.f, 150.f, 10.f)),
		params);
	if (Hit)
	{
		
		
		for (auto& result : resultArray) {
			if (result.Actor->GetActorLocation().Z > GetActorLocation().Z)
				continue;
			FVector Lo = result.Actor->GetActorLocation();
			Lo.Z = GetActorLocation().Z;
			Lo = GetActorLocation() - Lo;
			float Dis = Lo.Size();
			//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Distance %f"), Dis));
			if (result.Actor->IsA(AWallBuildable::StaticClass()) && Dis == 150.f)
			{
				AWallBuildable* Act = Cast<AWallBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(AWindowBuildable::StaticClass()) && Dis == 150.f)
			{
				AWindowBuildable* Act = Cast<AWindowBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(ADoorBuildable::StaticClass()) && Dis == 150.f)
			{
				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(ACeilingBuildable::StaticClass()) && Dis == 300.f)
			{
				ACeilingBuildable* Act = Cast<ACeilingBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
		}

		//PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Pillars %d"), Pillars.Num()));
	}
	if (Pillars.Num() <= 1)
	{
		BuildDestroy();
	}
}

void ACeilingBuildable::BuildChecking(FVector Location)
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


	if (Hit)
	{

		for (auto& result : resultArray) {
			if (result.Actor->IsA(AWallBuildable::StaticClass()))
			{
				AWallBuildable* Act = Cast<AWallBuildable>(result.Actor);
				FVector Lot = result.Location - Act->GetSceneComponent((int32)EWallSocket::CeilingSocketUp)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EWallSocket::CeilingSocketUp)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EWallSocket::CeilingSocketUp)->GetComponentRotation();
				for (int32 i = (int32)EWallSocket::CeilingSocketUp; i <= (int32)EWallSocket::CeilingSocketDown; i++)
				{
					Lot = result.Location - Act->GetSceneComponent(i)->GetComponentLocation();
					float Length2 = Lot.Size();
					if (Length1 > Length2)
					{
						Length1 = Length2;
						ResultLocation = Act->GetSceneComponent(i)->GetComponentLocation();
						ResultRotator = Act->GetSceneComponent(i)->GetComponentRotation();
					}
				}
				HitAmount--;

				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;

			}
			else if (result.Actor->IsA(AWindowBuildable::StaticClass()))
			{
				AWindowBuildable* Act = Cast<AWindowBuildable>(result.Actor);
				FVector Lot = result.Location - Act->GetSceneComponent((int32)EWindowSocket::CeilingSocket1)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EWindowSocket::CeilingSocket1)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EWindowSocket::CeilingSocket1)->GetComponentRotation();
				for (int32 i = (int32)EWindowSocket::CeilingSocket1; i <= (int32)EWindowSocket::CeilingSocket2; i++)
				{
					Lot = result.Location - Act->GetSceneComponent(i)->GetComponentLocation();
					float Length2 = Lot.Size();
					if (Length1 > Length2)
					{
						Length1 = Length2;
						ResultLocation = Act->GetSceneComponent(i)->GetComponentLocation();
						ResultRotator = Act->GetSceneComponent(i)->GetComponentRotation();
					}
				}
				HitAmount--;

				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
			else if (result.Actor->IsA(ADoorBuildable::StaticClass()))
			{
				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				FVector Lot = result.Location - Act->GetSceneComponent((int32)EDoorSocket::CeilingSocket1)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EDoorSocket::CeilingSocket1)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EDoorSocket::CeilingSocket1)->GetComponentRotation();
				for (int32 i = (int32)EDoorSocket::CeilingSocket1; i <= (int32)EDoorSocket::CeilingSocket2; i++)
				{
					Lot = result.Location - Act->GetSceneComponent(i)->GetComponentLocation();
					float Length2 = Lot.Size();
					if (Length1 > Length2)
					{
						Length1 = Length2;
						ResultLocation = Act->GetSceneComponent(i)->GetComponentLocation();
						ResultRotator = Act->GetSceneComponent(i)->GetComponentRotation();
					}
				}
				HitAmount--;

				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
		}
		bool Hit2 = GetWorld()->SweepMultiByChannel(resultArray,
			ResultLocation, ResultLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeBox(ActorSize),
			params);
		for (auto& result : resultArray) {
			if (result.Actor->IsA(ACeilingBuildable::StaticClass()))
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


