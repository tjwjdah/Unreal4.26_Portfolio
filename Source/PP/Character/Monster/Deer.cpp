// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"


ADeer::ADeer() {

	m_MonsterInfoName = TEXT("Deer");
}
void ADeer::BeginPlay()
{
	GetMesh()->CreateDynamicMaterialInstance(0);
	
	GetMesh()->CreateDynamicMaterialInstance(1);
	Super::BeginPlay();
}

void ADeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ADeer::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	return Damage;
}