// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowBuildable.h"
#include "DoorBuildable.h"
#include "WallBuildable.h"
#include "FoundationBuildable.h"
#include "CeilingBuildable.h"
AWindowBuildable::AWindowBuildable() {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BuildComponents = CreateDefaultSubobject<USceneComponent>(TEXT("BuildComponents"));
	SceneComponents.Add(BuildComponents);

	USceneComponent* WallSocket = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket"));
	SceneComponents.Add(WallSocket);
	USceneComponent* CeilingSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket1"));
	SceneComponents.Add(CeilingSocket1);
	USceneComponent* CeilingSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("CeilingSocket2"));
	SceneComponents.Add(CeilingSocket2);
	USceneComponent* TriangleCeilingSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket1"));
	SceneComponents.Add(TriangleCeilingSocket1);
	USceneComponent* TriangleCeilingSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("TriangleCeilingSocket2"));
	SceneComponents.Add(TriangleCeilingSocket2);
	USceneComponent* RoofSocket1 = CreateDefaultSubobject<USceneComponent>(TEXT("RoofSocket1"));
	SceneComponents.Add(RoofSocket1);
	USceneComponent* RoofSocket2 = CreateDefaultSubobject<USceneComponent>(TEXT("RoofSocket2"));
	SceneComponents.Add(RoofSocket2);
	USceneComponent* WindowSocket = CreateDefaultSubobject<USceneComponent>(TEXT("WindowSocket"));
	SceneComponents.Add(WindowSocket);
	CheckPropCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckPropCollision"));
	BuildComponents->SetupAttachment(RootComponent);
	CheckPropCollision->SetupAttachment(BuildComponents);
	for (int32 i = 1; i < SceneComponents.Num(); i++)
	{
		SceneComponents[i]->SetupAttachment(BuildComponents);
	}
}

void AWindowBuildable::BeginPlay()
{
	Super::BeginPlay();
}

void AWindowBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWindowBuildable::Build()
{
	Super::Build();
	TArray<FHitResult>	resultArray;

	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(ActorSize +20.f),
		params);
	if (Hit)
	{

		for (auto& result : resultArray) {
			if (result.Actor->IsA(AFoundationBuildable::StaticClass()))
			{
				AFoundationBuildable* Act = Cast<AFoundationBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(ACeilingBuildable::StaticClass()))
			{
				ACeilingBuildable* Act = Cast<ACeilingBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(AWallBuildable::StaticClass()))
			{
				AWallBuildable* Act = Cast<AWallBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(AWindowBuildable::StaticClass()))
			{
				AWindowBuildable* Act = Cast<AWindowBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
			else if (result.Actor->IsA(ADoorBuildable::StaticClass()))
			{
				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
		}
	}

}

void AWindowBuildable::BuildChecking(FVector Location)
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
			if (result.Actor->IsA(AFoundationBuildable::StaticClass()))
			{
				AFoundationBuildable* Act = Cast<AFoundationBuildable>(result.Actor);
				FVector Lot = result.Location - Act->GetSceneComponent((int32)EFoundationSocket::WallSocketUp)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EFoundationSocket::WallSocketUp)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EFoundationSocket::WallSocketUp)->GetComponentRotation();
				for (int32 i = (int32)EFoundationSocket::WallSocketUp; i <= (int32)EFoundationSocket::WallSocketLeft; i++)
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
			else if (result.Actor->IsA(AWallBuildable::StaticClass()))
			{
				AWallBuildable* Act = Cast<AWallBuildable>(result.Actor);
				ResultLocation = Act->GetSceneComponent((int32)EWallSocket::WallSocket)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EWallSocket::WallSocket)->GetComponentRotation();
				HitAmount--;
				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
			else if (result.Actor->IsA(AWindowBuildable::StaticClass()))
			{
				AWindowBuildable* Act = Cast<AWindowBuildable>(result.Actor);
				ResultLocation = Act->GetSceneComponent((int32)EWindowSocket::WallSocket)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EWindowSocket::WallSocket)->GetComponentRotation();
				HitAmount--;
				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
			else if (result.Actor->IsA(ADoorBuildable::StaticClass()))
			{
				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				ResultLocation = Act->GetSceneComponent((int32)EDoorSocket::WallSocket)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EDoorSocket::WallSocket)->GetComponentRotation();
				HitAmount--;
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
		if (Hit2) {
			for (auto& result : resultArray) {
				if (result.Actor->IsA(AWallBuildable::StaticClass()) || result.Actor->IsA(AWindowBuildable::StaticClass()) || result.Actor->IsA(ADoorBuildable::StaticClass()))
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

