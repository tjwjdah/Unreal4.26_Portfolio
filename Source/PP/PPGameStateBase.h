// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/GameStateBase.h"
#include "PPGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PP_API APPGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
private:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AChunkManager* ChunkManager;


public:
	AChunkManager* GetChunkManager();

	virtual void BeginPlay() override;
};
