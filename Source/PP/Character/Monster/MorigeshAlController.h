// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "MorigeshAlController.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMorigeshAlController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	AMorigeshAlController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
