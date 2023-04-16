// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescWidget.h"

void UQuestDescWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestNameText")));
	m_DescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestDescText")));
	m_CompensationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestCompensation")));
	m_CompleteList = Cast<UListView>(GetWidgetFromName(TEXT("CompleteList")));

	m_CompleteList->OnItemClicked().AddUObject(this, &UQuestDescWidget::QuestClick);
}
void UQuestDescWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
void UQuestDescWidget::QuestClick(UObject* Data)
{

}
