// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PlayerDetect.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UBTService_PlayerDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_PlayerDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
