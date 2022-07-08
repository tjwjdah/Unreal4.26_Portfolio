// Fill out your copyright notice in the Description page of Project Settings.


#include "StairsBuildable.h"
#include "FoundationBuildable.h"
#include "CeilingBuildable.h"
AStairsBuildable::AStairsBuildable() {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BuildComponents = CreateDefaultSubobject<USceneComponent>(TEXT("BuildComponents"));
	SceneComponents.Add(BuildComponents);
}

void AStairsBuildable::BeginPlay()
{
	Super::BeginPlay();
}

void AStairsBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AStairsBuildable::Build()
{
	Super::Build();

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
		}
	}
}

void AStairsBuildable::BuildChecking(FVector Location)
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
				ResultLocation = Act->GetSceneComponent((int32)EFoundationSocket::StairsSocket)->GetComponentLocation();
				//ResultRotator = Act->GetSceneComponent((int32)EFoundationSocket::StairsSocket)->GetComponentRotation();
				HitAmount--;
				resultArray.RemoveAll([](FHitResult v) {return true; });
				break;
			}
			else if (result.Actor->IsA(ACeilingBuildable::StaticClass()))
			{
				ACeilingBuildable* Act = Cast<ACeilingBuildable>(result.Actor);
				ResultLocation = Act->GetSceneComponent((int32)ECeilingSocket::StairsSocket)->GetComponentLocation();
				//ResultRotator = Act->GetSceneComponent((int32)EFoundationSocket::StairsSocket)->GetComponentRotation();
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
		for (auto& result : resultArray) {
			if (result.Actor->IsA(AStairsBuildable::StaticClass()))
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
	Super::BuildChecking(ResultLocation);

}
