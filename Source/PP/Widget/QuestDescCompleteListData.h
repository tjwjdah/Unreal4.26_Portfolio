// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestDescCompleteListData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PP_API UQuestDescCompleteListData : public UObject
{
	GENERATED_BODY()
public:
	UQuestDescCompleteListData();

private:
	FString m_TypeText;
	FString m_DestText;
	FString m_CountText;
	int32 m_Count;
	int32 m_MaxCount;
	int32 m_Index;
	class UQuestDescCompleteListWidget* m_OwnerWidget;
public:
	int32 GetIndex() const
	{
		return m_Index;
	}
	int32 GetCount() const
	{
		return m_Count;
	}
	int32 GetMaxCount() const
	{
		return m_MaxCount;
	}
	void SetIndex(int32 Index) {
		m_Index = Index;
	}

	FString GetTypeText() {
		return m_TypeText;
	}

	FString GetDestText() {
		return m_DestText;
	}
	FString GetCountText() {
		return m_CountText;
	}
	void SetTypeText(const FString& Text) {
		m_TypeText = Text;
	}
	
	void SetDestText(const FString& Text) {
		m_DestText = Text;
	}
	void SetCountText(int32 Count, int32 CountMax) {
		m_Count = Count;
		m_MaxCount = CountMax;
		if (m_Count == m_MaxCount)
			m_CountText = TEXT("완료");

		else
			m_CountText = FString::Printf(TEXT("%d / %d"), Count, CountMax);
	}
	void SetOwnerWidget(class UQuestDescCompleteListWidget* Widget)
	{
		m_OwnerWidget = Widget;
	}
};
