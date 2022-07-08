// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnim.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
	Death,
	Skill,
};

UENUM(BlueprintType)
enum class EMosnterAttackType : uint8
{
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	Attack5,
	Attack6
};

UCLASS()
class PP_API UMonsterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMonsterAnim();

protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterAnimType	m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMosnterAttackType	m_AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_AttackMax;




public:
	EMonsterAnimType GetAnimType()	const
	{
		return m_AnimType;
	}

	void ChangeAnimType(EMonsterAnimType Type)
	{
		m_AnimType = Type;
	}

	void SetHit()
	{
		m_Hit = 1.f;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
		void AnimNotify_Attack();

	UFUNCTION()
		void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_DeathEnd();

	UFUNCTION()
		void AnimNotify_HitEnd();


public:
	void SetAttackType(EMosnterAttackType _Type) {
		m_AttackType = _Type;
	}
	EMosnterAttackType GetAttackType() {
		return m_AttackType;
	}

	int32 GetAttackMax() { return m_AttackMax; }
};
