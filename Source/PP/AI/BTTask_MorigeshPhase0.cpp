// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MorigeshPhase0.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Morigesh.h"
#include "../Character/Monster/MorigeshAlController.h"
UBTTask_MorigeshPhase0::UBTTask_MorigeshPhase0() {
	NodeName = TEXT("MorigeshPhase0");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MorigeshPhase0::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMorigesh* Monster = Cast<AMorigesh>(Controller->GetPawn());

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
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Target);
	Controller->StopMovement();
	Monster->ChangeAnimType(EMonsterAnimType::Skill);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_MorigeshPhase0::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_MorigeshPhase0::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress로 해두었기 때문에 여기서 안나간다.
	// 그래서 공격거리 안에 들어왔는지를 여기서도 체크하여 들어갔다면
	// Trace를 종료시킨다.
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

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{

		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMorigesh* Morigesh = Cast<AMorigesh>(Controller->GetPawn());

	
	if (!Morigesh->GetSummonsZobiesCheck())
	{
			Morigesh->SetState(EState::End);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	return;
}
