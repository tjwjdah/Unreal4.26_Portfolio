// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnim.h"
#include "Monster.h"

UMonsterAnim::UMonsterAnim()
{
	m_OnGround = true;

	m_AnimType = EMonsterAnimType::Idle;

	m_Hit = 0.f;

	m_AttackMax = 1;
}

void UMonsterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
	{
		UCharacterMovementComponent* Movement = Monster->GetCharacterMovement();

		if (Movement)
		{
			// ¶¥À» ¹â°í ÀÖ´ÂÁö ±¸ÇÑ´Ù.
			bool OnGround = Movement->IsMovingOnGround();

			m_OnGround = OnGround;
		}
	}
}

void UMonsterAnim::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->NormalAttack();
}

void UMonsterAnim::AnimNotify_AttackEnd()
{
	//PrintViewport(1.f, FColor::Red, TEXT("AttackEnd"));
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if(Monster)
		Monster->SetAttackEnd(true);
	
}

void UMonsterAnim::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->Death();
}

void UMonsterAnim::AnimNotify_HitEnd()
{
	m_Hit = 0.f;
}
