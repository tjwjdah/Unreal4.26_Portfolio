// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleBuildable.h"

// Sets default values
ADestructibleBuildable::ADestructibleBuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Time=0.f;
	DestructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("BuildDestructibleMesh"));
	DestructibleMesh->SetupAttachment(RootComponent);
	DestructibleMesh->SetCollisionProfileName(TEXT("Destructible"));
}

// Called when the game starts or when spawned
void ADestructibleBuildable::BeginPlay()
{
	Super::BeginPlay();
	DestructibleMesh->ApplyDamage(100.f, GetActorLocation(), FVector(0.f, 0.f, -1.f), 100.f);
}

// Called every frame
void ADestructibleBuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Time += DeltaTime;

	if (Time >= 2.f) {
		Destroy();
		Time = 0.f;
	}

}

void ADestructibleBuildable::SetDestructibleMesh(UDestructibleMesh* Destructiblemesh)
{
	DestructibleMesh->SetDestructibleMesh(Destructiblemesh);
}
