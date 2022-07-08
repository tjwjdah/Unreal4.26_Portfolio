// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드를 지정한다.
	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		// 행동트리를 동작시킨다.
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Monster BehaviorTree Error!!"));
		}
	}
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
