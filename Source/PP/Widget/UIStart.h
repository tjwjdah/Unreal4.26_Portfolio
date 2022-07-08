// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UIButton.h"
#include "Blueprint/UserWidget.h"
#include "UIStart.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UUIStart : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUIButton* m_BtnEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetAnimation* m_StartAnimation;


	float	m_CameraMoveTime;
	bool    m_CameraMove;

	int32    m_Test;
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	
	void BindingAnimations();

	UFUNCTION()
	void StartBtn();
	UFUNCTION()
	void EndBtn();

	void SetPlayer();
};
