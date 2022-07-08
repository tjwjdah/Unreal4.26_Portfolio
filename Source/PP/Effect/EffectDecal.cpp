// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDecal.h"

// Sets default values
AEffectDecal::AEffectDecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	SetRootComponent(m_Decal);

	m_Decal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));



}

// Called when the game starts or when spawned
void AEffectDecal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEffectDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



