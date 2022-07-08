// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_CrosshairImage;

	UCanvasPanelSlot* PanelSlot;

	FVector2D ScreenPosition;
public:
	void SetCrosshairPosition(FVector _WorldLocation);
	
};
