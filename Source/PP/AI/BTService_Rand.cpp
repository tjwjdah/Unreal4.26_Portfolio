// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Rand.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Monster.h"
#include "../Character/Monster/MonsterAIController.h"

UBTService_Rand::UBTService_Rand() {
	NodeName = TEXT("Rand");
	Interval = 1.f;
}

void UBTService_Rand::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!Controller)
		return;

	
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return;

	
	if (m_RandPercentage.Num() <= 0)
		return;

	float fRand = FMath::SRand();

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Rand : %f"), fRand));
	float fPercentage = 0.f;
	for (int32 i = 0; i < m_RandPercentage.Num(); i++)
	{
		fPercentage += m_RandPercentage[i];
		if (fPercentage >= fRand)
		{
			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Percentage : %f"), fPercentage));
			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("i : %d"), i));
			Controller->GetBlackboardComponent()->SetValueAsInt(TEXT("Rand"), i);
			Monster->SetAttackType(i);
			return;
		}

	}
	

	return;
}

