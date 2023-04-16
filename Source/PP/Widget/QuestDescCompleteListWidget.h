// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescCompleteListWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UQuestDescCompleteListWidget : public UUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_TypeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_DestText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CountText;

	int32 m_Index;
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	UFUNCTION(BlueprintCallable)
		void SetData(UObject* Data);

	int32 GetIndex() {
		return m_Index;
	}
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);
	virtual void NativeOnItemSelectionChanged(bool bIsSelected);
};
