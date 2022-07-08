// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_MorigeshPhase2.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UUBTTask_MorigeshPhase2 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTTask_MorigeshPhase2();


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
