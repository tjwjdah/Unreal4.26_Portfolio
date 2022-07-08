// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "ZoobieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AZoobieAIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
		AZoobieAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
