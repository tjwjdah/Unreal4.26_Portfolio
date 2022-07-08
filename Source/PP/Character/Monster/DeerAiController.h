// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "DeerAiController.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ADeerAiController : public AMonsterAIController
{
	GENERATED_BODY()
	
public:
	ADeerAiController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
