// Fill out your copyright notice in the Description page of Project Settings.


#include "MorigeshAnim.h"
#include "Morigesh.h"

UMorigeshAnim::UMorigeshAnim() {
	
	m_Phase = EPhase::Phase0;

	m_State = EState::Start;
	m_AttackMax = 4;
}

void UMorigeshAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMorigeshAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMorigeshAnim::AnimNotify_SummonsZoobie()
{
	AMorigesh* Monster = Cast<AMorigesh>(TryGetPawnOwner());
	if (m_Phase == EPhase::Phase0)
	{
		
		Monster->SummonsZombies();
		
	}
	else if(m_Phase == EPhase::Phase1 || (m_Phase == EPhase::Phase2 && m_AttackType == EMosnterAttackType::Attack2))
	{
		Monster->SummonsExplosion();
	}
	else if (m_Phase == EPhase::Phase2 && m_AttackType == EMosnterAttackType::Attack3)
	{
		Monster->SetMeteorOn(true);
	}
}

void UMorigeshAnim::AnimNotify_SummonsZoobieing()
{
	m_State = EState::Ing;
	if (m_Phase == EPhase::Phase1 || (m_Phase == EPhase::Phase2 && m_AttackType == EMosnterAttackType::Attack2))
	{
		AMorigesh* Monster = Cast<AMorigesh>(TryGetPawnOwner());
		if (Monster)
			Monster->ExplosionStart();
	}
}

void UMorigeshAnim::AnimNotify_Phase0End()
{
	m_State = EState::Start;

	AMorigesh* Monster = Cast<AMorigesh>(TryGetPawnOwner());
	Monster->SetPhase(EPhase::Phase1);
	Monster->SummonsZombieEnd();
}

void UMorigeshAnim::AnimNotify_DaggerAttackEnd()
{

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (Monster)
		Monster->SetAttackEnd(true);
}

void UMorigeshAnim::AnimNotify_DaggerAttack()
{
	
	AMorigesh* Monster = Cast<AMorigesh>(TryGetPawnOwner());
	Monster->DaggerAttack();
	
}

void UMorigeshAnim::AnimNotify_ExplosionEnd()
{
	m_State = EState::Start;

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (Monster)
	{
		Monster->SetAttackEnd(true);
		
	}

}

void UMorigeshAnim::AnimNotify_ShieldOn()
{
	AMorigesh* Monster = Cast<AMorigesh>(TryGetPawnOwner());
	if (Monster)
	{
		Monster->ShieldSummon();
	}
}

void UMorigeshAnim::AnimNotify_ShieldEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (Monster)
	{
		Monster->SetAttackEnd(true);
	}
}
