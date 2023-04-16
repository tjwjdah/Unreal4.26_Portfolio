// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "QuestDescCompleteListData.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UQuestDescWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_DescText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_CompensationText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_CompleteList;


public:
	void SetQuestName(const FString& Name) {
		m_NameText->SetText(FText::FromString(Name));
	}

	void SetQuestDesc(const FString& Desc) {
		m_DescText->SetText(FText::FromString(Desc));
	}
	void SetCompensationName(const FString& Name) {
		m_CompensationText->SetText(FText::FromString(Name));
	}
	FText GetCompensationName() {
		return m_CompensationText->GetText();
	}
	void ClearList() {
		m_CompleteList->ClearListItems();
	}
	void AddList(const FQuestDataInfo& Info,int32 Index) {
		UQuestDescCompleteListData* QuestItemData = NewObject<UQuestDescCompleteListData>(this,
			UQuestDescCompleteListData::StaticClass());

		FString TypeText;
		switch (Info.Type)
		{
		case EQuestType::Collection:
			TypeText = "Collection";
			break;
		case EQuestType::Hunt:
			TypeText = "Hunt";
			break;
		case EQuestType::Craft:
			TypeText = "Craft";
			break;
		
		}
		QuestItemData->SetTypeText(TypeText);
		QuestItemData->SetDestText(Info.DestName);
		QuestItemData->SetIndex(Index);
		QuestItemData->SetCountText(Info.Count, Info.MaxCount);
		Index++;

		m_CompleteList->AddItem(QuestItemData);
	}
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void QuestClick(UObject* Data);
};
