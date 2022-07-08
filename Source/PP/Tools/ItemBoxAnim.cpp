// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBoxAnim.h"


UItemBoxAnim::UItemBoxAnim() {
	m_AnimType = EBoxAnimType::Closed;

}

void UItemBoxAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UItemBoxAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UItemBoxAnim::AnimNotify_Close()
{
	m_AnimType = EBoxAnimType::Closed;
	//CurrentLifeTimerScrubPosition
}

void UItemBoxAnim::AnimNotify_Open()
{
	m_AnimType = EBoxAnimType::Opened;
}
