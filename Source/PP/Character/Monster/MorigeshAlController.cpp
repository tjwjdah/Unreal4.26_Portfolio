// Fill out your copyright notice in the Description page of Project Settings.


#include "MorigeshAlController.h"

AMorigeshAlController::AMorigeshAlController() {
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Character/Monster/AI/BTMorigesh.BTMorigesh'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Character/Monster/AI/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void AMorigeshAlController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMorigeshAlController::OnUnPossess()
{
	Super::OnUnPossess();
}
