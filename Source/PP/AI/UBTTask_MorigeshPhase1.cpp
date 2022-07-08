// Fill out your copyright notice in the Description page of Project Settings.


#include "UBTTask_MorigeshPhase1.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Character/Monster/Morigesh.h"
#include "../Character/Monster/MorigeshAlController.h"
UUBTTask_MorigeshPhase1::UUBTTask_MorigeshPhase1() {
	NodeName = TEXT("MorigeshPhase1");
	bNotifyTick = true;
}

EBTNodeResult::Type UUBTTask_MorigeshPhase1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	Monster->ChangeAnimType(EMonsterAnimType::Skill);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UUBTTask_MorigeshPhase1::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UUBTTask_MorigeshPhase1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMorigesh* Monster = Cast<AMorigesh>(Controller->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//PrintViewport(1.f, FColor::Red, TEXT("NormalAttack AttackStart"));


	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{

		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 타겟과의 거리를 구한다.
	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	MonsterLoc.Z = TargetLoc.Z;

	float	Distance = FVector::Distance(MonsterLoc, TargetLoc);


	if (Monster->GetAttackEnd())
	{
	
		Monster->SetState(EState::Start);
		//Monster->RandChangeAttack(0,1);
		FVector	Dir = TargetLoc - MonsterLoc;
		Dir.Normalize();

		Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		/*
		if (Distance > MonsterInfo.AttackInfos[Monster->GetAttackType()].AttackDistance)
		{
		
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		else
		{
			FVector	Dir = TargetLoc - MonsterLoc;
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
			
		}
		*/
		Monster->SetAttackEnd(false);
		return;
	}

}
