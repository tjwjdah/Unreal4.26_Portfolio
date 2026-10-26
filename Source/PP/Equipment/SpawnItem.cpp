// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnItem.h"
#include "../Character/Player/PlayerCharacter.h"
// Sets default values
ASpawnItem::ASpawnItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
}

// Called when the game starts or when spawned
void ASpawnItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASpawnItem::SetPlayerOwner(APlayerCharacter* _Owner)
{
	m_Owner = _Owner;
}
void ASpawnItem::SetMesh(USkeletalMesh* _Mesh)
{
	m_Mesh->SetSkeletalMesh(_Mesh);
}

