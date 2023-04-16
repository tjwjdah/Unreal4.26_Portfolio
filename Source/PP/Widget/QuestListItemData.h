// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

#include "UObject/NoExportTypes.h"
#include "QuestListItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PP_API UQuestListItemData : public UObject
{
	GENERATED_BODY()
	
public:
	UQuestListItemData();

private:
	FString m_Name;
	int32 m_Index;
	FLinearColor m_Color;
	class UQuestListItemWidget* m_OwnerWidget;
public:
	void SetIndex(int32 Index) {
		m_Index = Index;
	}
	int32 GetIndex() {
		return m_Index;
	}

	void SetQuestName(const FString& Name) {
		m_Name = Name;
	}

	const FString& GetQuestName() const {
		return m_Name;
	}
	void SetColor(FLinearColor _Color) { m_Color = _Color; }
	FLinearColor GetColor() { return m_Color; }

	class UQuestListItemWidget* GetOwnerWidget()
	{
		return m_OwnerWidget;
	}
	void SetOwnerWidget(class UQuestListItemWidget* Widget)
	{
		m_OwnerWidget = Widget;
	}
};
