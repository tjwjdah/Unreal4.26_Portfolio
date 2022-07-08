// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ParticleProjectile.h"
#include "MorigeshExplosion.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMorigeshExplosion : public AParticleProjectile
{
	GENERATED_BODY()

public:
	AMorigeshExplosion();

private:
	int32	m_Attack;
	class AMonster* m_Owner;
	AActor* m_Player;

	FRotator m_Velocity;


	bool	m_Start;

	float   m_ExplosionTime;
public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AMonster* Monster)
	{
		m_Owner = Monster;
	}

	void SetPlayer(AActor* _Player)
	{
		m_Player = _Player;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);


public:
	void Explosion();
	void Start();
};
