// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescCompleteListWidget.h"
#include "QuestDescCompleteListData.h"
void UQuestDescCompleteListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	m_TypeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TypeText")));;
	m_DestText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DestText")));;
	m_CountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountText")));
}
void UQuestDescCompleteListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
void UQuestDescCompleteListWidget::SetData(UObject* Data)
{
	UQuestDescCompleteListData* Item = Cast<UQuestDescCompleteListData>(Data);
	m_TypeText->SetText(FText::FromString(Item->GetTypeText()));
	m_DestText->SetText(FText::FromString(Item->GetDestText()));
	m_CountText->SetText(FText::FromString(Item->GetCountText()));
}

void UQuestDescCompleteListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UQuestDescCompleteListData* Item = Cast<UQuestDescCompleteListData>(ListItemObject);
	m_TypeText->SetText(FText::FromString(Item->GetTypeText()));
	m_DestText->SetText(FText::FromString(Item->GetDestText()));
	m_CountText->SetText(FText::FromString(Item->GetCountText()));
	m_Index = Item->GetIndex();
}

void UQuestDescCompleteListWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	
}
