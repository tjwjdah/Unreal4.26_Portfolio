// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoobieAIController.h"

AZoobieAIController::AZoobieAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Character/Monster/AI/BTZoobie.BTZoobie'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Character/Monster/AI/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void AZoobieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AZoobieAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
