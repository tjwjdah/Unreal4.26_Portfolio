// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListItemWidget.h"

void UQuestListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	m_BackgroundIcon = Cast<UImage>(GetWidgetFromName(TEXT("BackgroundIcon")));
	
}
void UQuestListItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestListItemWidget::SetData(UObject* Data)
{
	UQuestListItemData* Item = Cast<UQuestListItemData>(Data);
	m_NameText->SetText(FText::FromString(Item->GetQuestName()));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), Item->GetColor().R, Item->GetColor().G, Item->GetColor().A));
	m_BackgroundIcon->SetColorAndOpacity(Item->GetColor());
}
