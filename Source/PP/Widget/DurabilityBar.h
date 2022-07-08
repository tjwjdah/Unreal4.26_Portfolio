// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "DurabilityBar.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UDurabilityBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* m_DurabilityBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Text;


public:
	void SetDurabilityPercent(float Percent)
	{
		m_DurabilityBar->SetPercent(Percent);
	}
	void SetText(FText Text)
	{
		m_Text->SetText(Text);
	}
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};
