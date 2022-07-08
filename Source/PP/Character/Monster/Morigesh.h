// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Monster.h"
#include "Morigesh.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMorigesh : public AMonster
{
	GENERATED_BODY()

public:
		AMorigesh();

private:
	EPhase m_Phase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AZoobie>	m_ZoobieClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class AZoobie*> m_SummonsZoobies;
		

	class ANormalEffect* m_Circle;

	class UMorigeshAnim* m_MorigeshAnim;

	class AMorigeshExplosion* m_Explosion;

	bool  m_SummonsZobiesCheck;

	int32 m_MeteorNumber;
	float m_MeteorTime;
	bool m_MeteorOn;


	bool m_ShieldOn;
	class AShield* m_Shield;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	virtual void NormalAttack();
	virtual void DeathEffect();
public:
	EPhase GetPhase() { return m_Phase; }
	void SetPhase(EPhase _Phase);
	void SetState(EState _State);
	EState GetState();
	void SummonsZombies();
	void SummonsZombieEnd();
	void SummonsExplosion();
	void ExplosionStart();
	void Explosion();
	bool GetSummonsZobiesCheck() { return m_SummonsZobiesCheck; };
	

	void DaggerAttack();

	void SummonsMeteor();
	void SetMeteorOn(bool _OnOff) {
		m_MeteorOn = _OnOff;
	};
	bool GetShieldOn() { return m_ShieldOn; }
	class AShield* GetShield();
	void ShieldSummon();

	void DeathOn();
};
