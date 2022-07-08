// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UIButton.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UUIButton : public UUserWidget
{
	GENERATED_BODY()

protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_Button;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	UButton* GetButton() { return m_Button; }
};
