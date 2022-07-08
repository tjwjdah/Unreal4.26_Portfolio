// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Monster.h"
#include "Zoobie.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AZoobie : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZoobie();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_Summon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void Summon();
	void SetSummon(bool _Summon) { m_Summon = _Summon; }
};
