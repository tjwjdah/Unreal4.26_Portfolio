// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorBuildable.h"
#include "DoorBuildable.h"
AOpenDoorBuildable::AOpenDoorBuildable() {
	PrimaryActorTick.bCanEverTick = true;
	
}

void AOpenDoorBuildable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOpenDoorBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOpenDoorBuildable::Build()
{
	Super::Build();
	if (FrameReference)
	{
		FrameReference->SetDoor(this);
		AttachToComponent(FrameReference->GetSceneComponent((int32)EDoorSocket::DoorSocket), FAttachmentTransformRules::KeepWorldTransform);
	}

	TArray<FHitResult>	resultArray;

	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	bool Hit = GetWorld()->SweepMultiByChannel(resultArray,
		GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(ActorSize + 20.f),
		params);
	if (Hit)
	{

		for (auto& result : resultArray) {
			if (result.Actor->IsA(ADoorBuildable::StaticClass()))
			{
				ADoorBuildable* Act = Cast<ADoorBuildable>(result.Actor);
				AddPillar(Act);
				Act->AddCeilings(this);
			}
		}
	}
}

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

void AOpenDoorBuildable::BuildDestroy()
{
	Super::BuildDestroy();
	FrameReference->SetDoor(nullptr);
}

