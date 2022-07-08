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

	// InProgress�� �صξ��� ������ ���⼭ �ȳ�����.
	// �׷��� ���ݰŸ� �ȿ� ���Դ����� ���⼭�� üũ�Ͽ� ���ٸ�
	// Trace�� �����Ų��.
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
