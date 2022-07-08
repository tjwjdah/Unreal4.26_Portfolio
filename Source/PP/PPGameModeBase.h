// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "../PP/Widget/MainHUD.h"

#include "PPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PP_API APPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	APPGameModeBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMainHUD>	m_MainHUDClass;
	UMainHUD* m_MainHUD;
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;


public:
	UMainHUD* GetMainHUD()
	{
		return m_MainHUD;
	}


};
