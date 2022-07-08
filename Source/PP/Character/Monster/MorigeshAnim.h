// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MonsterAnim.h"
#include "MorigeshAnim.generated.h"





/**
 * 
 */
UCLASS()
class PP_API UMorigeshAnim : public UMonsterAnim
{
	GENERATED_BODY()

public:
	UMorigeshAnim();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPhase	m_Phase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EState	m_State;

	
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);


public:
	void SetPhase(EPhase _Phase) {
		m_Phase = _Phase;
	}
	void SetState(EState _State) {
		m_State = _State;
	}

	EState GetState() {
		return m_State;
	}
public:
	UFUNCTION()
		void AnimNotify_SummonsZoobie();

	UFUNCTION()
		void AnimNotify_SummonsZoobieing();

	UFUNCTION()
		void AnimNotify_Phase0End();

	UFUNCTION()
		void AnimNotify_DaggerAttackEnd();
	UFUNCTION()
		void AnimNotify_DaggerAttack();

	UFUNCTION()
		void AnimNotify_ExplosionEnd();
	UFUNCTION()
		void AnimNotify_ShieldOn();
	UFUNCTION()
		void AnimNotify_ShieldEnd();
	
};
