// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckPhase.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UBTDecorator_CheckPhase : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckPhase();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPhase m_Phase;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
