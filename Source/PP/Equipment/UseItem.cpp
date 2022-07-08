// Fill out your copyright notice in the Description page of Project Settings.


#include "UseItem.h"
#include "../Character/Player/PlayerCharacter.h"
// Sets default values
AUseItem::AUseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AUseItem::SetOwner(APlayerCharacter* _Owner)
{
	m_Owner = _Owner;
}
