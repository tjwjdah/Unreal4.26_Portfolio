// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../GameInfo.h"
#include "Monster.h"
#include "Deer.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ADeer : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADeer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
public:
	

};
