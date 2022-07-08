// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Getaway.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Monster.h"
#include "../Character/Monster/MonsterAIController.h"

UBTTask_Getaway::UBTTask_Getaway()
{
	NodeName = TEXT("Getaway");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Getaway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		return EBTNodeResult::Failed;
	}
	
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterInfo().MoveSpeed;
	//UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Target);
	
	GetAwayVector = -(Target->GetActorLocation() - Monster->GetActorLocation());
	GetAwayVector.Normalize();
	
	Monster->ChangeAnimType(EMonsterAnimType::Run);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Getaway::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Getaway::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	m_fGetawayTime += DeltaSeconds;

	FVector MoveVector = Monster->GetActorLocation();
	MoveVector.X += GetAwayVector.X * DeltaSeconds * 20000.f;
	MoveVector.Y += GetAwayVector.Y * DeltaSeconds * 20000.f;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, MoveVector);

	if (m_fGetawayTime > 10.f)
	{
		m_fGetawayTime = 0.f;
		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	/*
	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	*/
	//const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 타겟과의 거리를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	//FVector	TargetLoc = Target->GetActorLocation();
	/*
	MonsterLoc.Z = TargetLoc.Z;

	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);


	if (Distance <= MonsterInfo.AttackDistance)
	{
		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	*/
}
