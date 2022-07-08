// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ParticleProjectile.h"
#include "MorigeshDagger.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMorigeshDagger : public AParticleProjectile
{
	GENERATED_BODY()
	
public:
	AMorigeshDagger();

private:
	int32	m_Attack;
	class AMonster* m_Owner;

	float	m_DestroyTime;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AMonster* Monster)
	{
		m_Owner = Monster;
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
