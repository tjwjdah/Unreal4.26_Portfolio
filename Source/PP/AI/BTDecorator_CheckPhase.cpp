// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPhase.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Monster.h"
#include "../Character/Monster/Morigesh.h"
#include "../Character/Monster/MonsterAIController.h"


UBTDecorator_CheckPhase::UBTDecorator_CheckPhase()
{
	NodeName = TEXT("CheckPhase");
}

bool UBTDecorator_CheckPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	//PrintViewport(10.f, FColor::Blue, TEXT("dsadsad"));
	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	AMorigesh* Morigesh = Cast<AMorigesh>(Monster);

	if(!Morigesh)
		return false;

	// Ÿ�ٰ��� �Ÿ��� ���Ѵ�.
	
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Phase : %d"), Morigesh->GetPhase()));
	return (m_Phase == Morigesh->GetPhase());

}
