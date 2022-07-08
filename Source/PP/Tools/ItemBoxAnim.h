// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "ItemBoxAnim.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBoxAnimType : uint8
{
	Close,
	Closed,
	Open,
	Opened
};


UCLASS()
class PP_API UItemBoxAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UItemBoxAnim();

protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EBoxAnimType	m_AnimType;
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	EBoxAnimType GetAnimType()	const
	{
		return m_AnimType;
	}

	void ChangeAnimType(EBoxAnimType Type)
	{
		m_AnimType = Type;
	}

	UFUNCTION()
		void AnimNotify_Close();

	UFUNCTION()
		void AnimNotify_Open();
};
