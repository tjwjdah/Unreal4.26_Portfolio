// Fill out your copyright notice in the Description page of Project Settings.


#include "RampBuildable.h"
#include "FoundationBuildable.h"
ARampBuildable::ARampBuildable() {
	PrimaryActorTick.bCanEverTick = true;

	
}

void ARampBuildable::BeginPlay()
{
	Super::BeginPlay();
}

void ARampBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARampBuildable::Build()
{
	Super::Build();
	//CollisionVolume->SetCollisionProfileName(TEXT("PlayerIgnore"));
}

void ARampBuildable::BuildChecking(FVector Location)
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
				FVector Lot = result.Location - Act->GetSceneComponent((int32)EFoundationSocket::RampSocketUp)->GetComponentLocation();
				float Length1 = Lot.Size();
				ResultLocation = Act->GetSceneComponent((int32)EFoundationSocket::RampSocketUp)->GetComponentLocation();
				ResultRotator = Act->GetSceneComponent((int32)EFoundationSocket::RampSocketUp)->GetComponentRotation();
				for (int32 i = (int32)EFoundationSocket::RampSocketUp; i <= (int32)EFoundationSocket::RampSocketLeft; i++)
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
			if (result.Actor->IsA(ARampBuildable::StaticClass()) || result.Actor->IsA(AFoundationBuildable::StaticClass()))
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
