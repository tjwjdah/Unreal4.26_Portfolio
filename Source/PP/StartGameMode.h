// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "../PP/Widget/UIStart.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AStartGameMode();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUIStart>	m_StartHUDClass;

	UUIStart* m_StartHUD;

public:
	
	UUIStart* GetStartHUD()
	{
		return m_StartHUD;
	}

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
};
