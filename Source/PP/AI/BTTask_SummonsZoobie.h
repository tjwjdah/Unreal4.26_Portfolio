// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SummonsZoobie.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UBTTask_SummonsZoobie : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTTask_SummonsZoobie();
	~UBTTask_SummonsZoobie();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
