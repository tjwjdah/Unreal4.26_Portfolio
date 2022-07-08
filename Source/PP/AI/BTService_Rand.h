// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Rand.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UBTService_Rand : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Rand();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> m_RandPercentage;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
