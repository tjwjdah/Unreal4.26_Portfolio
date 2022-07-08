// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleProjectile.h"
#include "MorigeshMeteor.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMorigeshMeteor : public AParticleProjectile
{
	GENERATED_BODY()
	
public:
	AMorigeshMeteor();


private:
	int32	m_Attack;
	class AMonster* m_Owner;
	FVector m_TargetVector;
	FRotator m_Velocity;

	AActor* m_DecalEffect;

	float	m_Start;
public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AMonster* Monster)
	{

		m_Owner = Monster;
	}


	void SetTargetVector(FVector _Vector)
	{
		m_TargetVector = _Vector;
	}

	void SetDecal(AActor* _Decal)
	{
		m_DecalEffect = _Decal;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);

};
