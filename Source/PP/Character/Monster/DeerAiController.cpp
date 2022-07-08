// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerAiController.h"


ADeerAiController::ADeerAiController() {
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Character/Monster/AI/BTDeer.BTDeer'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Character/Monster/AI/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;

	
}

void ADeerAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ADeerAiController::OnUnPossess()
{
	Super::OnUnPossess();
}
