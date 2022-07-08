// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "HotKey.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UHotKey : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UEditableTextBox* m_Text;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetKeyText(FText _Key, FText _Text);
	
};
