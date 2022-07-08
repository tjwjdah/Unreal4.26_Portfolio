// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceBuildable.h"

AFenceBuildable::AFenceBuildable() {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BuildComponents = CreateDefaultSubobject<USceneComponent>(TEXT("BuildComponents"));
	SceneComponents.Add(BuildComponents);

	USceneComponent* WallSocket = CreateDefaultSubobject<USceneComponent>(TEXT("WallSocket"));
	SceneComponents.Add(WallSocket);
}

void AFenceBuildable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFenceBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFenceBuildable::Build()
{
	Super::Build();
}

void AFenceBuildable::BuildChecking(FVector Location)
{
	ResultLocation = Location;
	HitAmount = 0;
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

		
		ResultLocation.Z = Location.Z + ActorSize.Z;
			
		bool Hit2 = GetWorld()->SweepMultiByChannel(resultArray,
			ResultLocation, ResultLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeBox(ActorSize),
			params);
		for (auto& result : resultArray) {
			if (result.Actor->IsA(ABuildable::StaticClass()))
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
