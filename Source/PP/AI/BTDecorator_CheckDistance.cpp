// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Monster.h"
#include "../Character/Monster/MonsterAIController.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	//PrintViewport(10.f, FColor::Blue, TEXT("dsadsad"));
	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;
	
	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return false;

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 타겟과의 거리를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc.Z = TargetLoc.Z;

	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);
	float	CheckDist = 0.f;
	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("Distance : %f"), Distance));
	switch (m_CheckType)
	{
	case ECheckDistanceType::Trace:
		CheckDist = MonsterInfo.TraceDistance;
		break;
	case ECheckDistanceType::Attack:
		CheckDist = MonsterInfo.AttackInfos[Monster->GetAttackType()].AttackDistance;
		break;
	}
	
	return Distance <= CheckDist;
}