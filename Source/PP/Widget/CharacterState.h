// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

#include "CharacterState.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UCharacterState : public UUserWidget
{
	GENERATED_BODY()

protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_HealthBar;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
			UProgressBar* m_EnergyBar;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
			UProgressBar* m_HungerBar;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
			UProgressBar* m_ThirstBar;

public:
	void SetHealthPercent(float Percent)
	{
		m_HealthBar->SetPercent(Percent);
	}

	void SetEnergyPercent(float Percent)
	{
		m_EnergyBar->SetPercent(Percent);
	}
	void SetHungerPercent(float Percent)
	{
		m_HungerBar->SetPercent(Percent);
	}
	void SetThirstPercent(float Percent)
	{
		m_ThirstBar->SetPercent(Percent);
	}


protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);



	
};
