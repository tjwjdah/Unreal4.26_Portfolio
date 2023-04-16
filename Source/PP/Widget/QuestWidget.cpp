// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include "../PPGameInstance.h"
#include "QuestListItemData.h"
#include "../PPGameModeBase.h"

void UQuestWidget::NativeConstruct()
{

	Super::NativeConstruct();
	m_QuestList = Cast<UListView>(GetWidgetFromName(TEXT("QuestList")));
	m_QuestDesc = Cast<UQuestDescWidget>(GetWidgetFromName(TEXT("UI_QuestDesc")));

	m_QuestList->OnItemClicked().AddUObject(this, &UQuestWidget::QuestClick);
	m_QuestList->OnItemSelectionChanged().AddUObject(this, &UQuestWidget::QuestSelect);
	m_QuestList->OnItemIsHoveredChanged().AddUObject(this, &UQuestWidget::QuestMouseOn);
	m_CurrentRenderQuest = nullptr;
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	m_PreIndex = 0;
	if (GameInst) {
	    TMap<FString, FQuestData>& QuestData = GameInst->GetWQuestData();
		int32 Index = 0;
		for (auto& Data : QuestData) {
			const FQuestTableInfo* Info = GameInst->FindUIQuestInfo(Data.Key);

			if (Info) {
				Data.Value.Index = Index;
				m_QuestArray.Add(Data.Value);

				int32 In = m_QuestArray.Num() - 1;
				for (auto& QuestInfo : Info->InfoArray) {
					m_QuestListArray[(uint8)QuestInfo.Type].Add(&m_QuestArray[In]);
				}
				UQuestListItemData* QuestItemData = NewObject<UQuestListItemData>(this,
					UQuestListItemData::StaticClass());

				QuestItemData->SetQuestName(Info->Name);
				QuestItemData->SetIndex(Index);
				Index++;
				
				m_QuestList->AddItem(QuestItemData);
			}
		}
	}
}

void UQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestWidget::QuestClick(UObject* Data)
{
	UQuestListItemData* Info = Cast<UQuestListItemData>(Data);
	if (Info) {
		if (m_CurrentRenderQuest == &m_QuestArray[Info->GetIndex()])
			return;
		m_CurrentRenderQuest = &m_QuestArray[Info->GetIndex()];

		FString	QuestName = m_QuestArray[Info->GetIndex()].Name;

		FString CompensationS = TEXT("보상 :");
		for (int32 i = 0; i < m_QuestArray[Info->GetIndex()].CompensationArray.Num(); i++) {
			if (i != m_QuestArray[Info->GetIndex()].CompensationArray.Num() - 1)
				CompensationS += m_QuestArray[Info->GetIndex()].CompensationArray[i].ItemName
				+ FString::Printf(TEXT("%d개, "), m_QuestArray[Info->GetIndex()].CompensationArray[i].CompensationMount);
			else
				CompensationS += m_QuestArray[Info->GetIndex()].CompensationArray[i].ItemName
				+ FString::Printf(TEXT("%d개"), m_QuestArray[Info->GetIndex()].CompensationArray[i].CompensationMount);
		}

		if (m_CurrentRenderQuest->Complete) {
			QuestName = m_QuestArray[Info->GetIndex()].Name + TEXT("(완료)");
			CompensationS+= TEXT("(완료)");
		}

		UQuestListItemData* Item = Cast<UQuestListItemData>(m_QuestList->GetItemAt(m_PreIndex));
		Item->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		Info->SetColor(FLinearColor(1.f, 1.f, 1.0f, 1.0f));

		
		m_QuestList->RegenerateAllEntries();
		m_QuestDesc->SetQuestName(QuestName);
		

		m_QuestDesc->SetCompensationName(CompensationS);
		m_QuestDesc->SetQuestDesc(m_QuestArray[Info->GetIndex()].QuestDesc);
		m_QuestDesc->ClearList();
		m_PreIndex = Info->GetIndex();
		int32 Index = 0;
		for (auto& DataInfo : m_QuestArray[Info->GetIndex()].CompleteArray) {
			m_QuestDesc->AddList(DataInfo, Index);
			Index++;
		}
	}
}

void UQuestWidget::QuestSelect(UObject* Data)
{
}

void UQuestWidget::QuestMouseOn(UObject* Data, bool IsHovered)
{
}

void UQuestWidget::QuestCheck(EQuestType Type, const FString& Name)
{
	for (auto& Info : m_QuestListArray[(uint8)Type]) {
		if (Info->Complete)
			continue;

		int32	CompleteCount = 0;

		for (auto& QuestData : Info->CompleteArray) {
			if (QuestData.Complete)
			{
				++CompleteCount;
				continue;
			}

			if (QuestData.Type == Type && QuestData.DestName == Name)
			{
				++QuestData.Count;

				if (QuestData.Count == QuestData.MaxCount)
				{
					QuestData.Complete = true;
					++CompleteCount;
				}

				// 현재 퀘스트창에 보여지고 있는 퀘스트가 변경이 일어났을 경우
				// 갱신한다.
				if (m_CurrentRenderQuest == Info)
				{
					m_QuestDesc->SetQuestName(m_CurrentRenderQuest->Name);
					m_QuestDesc->SetQuestDesc(m_CurrentRenderQuest->QuestDesc);

					m_QuestDesc->ClearList();

					int32	CompleteIndex = 0;

					for (auto& DataInfo : m_CurrentRenderQuest->CompleteArray)
					{
						m_QuestDesc->AddList(DataInfo, CompleteIndex);
						++CompleteIndex;
					}
				}

				break;
			}
		}

		if (CompleteCount == Info->CompleteArray.Num()) {
			Info->Complete = true;
			UQuestListItemData* Item = Cast<UQuestListItemData>(m_QuestList->GetItemAt(Info->Index));
			Item->SetQuestName(Item->GetQuestName() + TEXT("(완료)"));
			m_QuestDesc->SetQuestName(Info->Name + TEXT("(완료)"));
			FString CompensationS = m_QuestDesc->GetCompensationName().ToString();
			CompensationS += TEXT("(완료)");
			m_QuestDesc->SetCompensationName(CompensationS);
			m_QuestList->RegenerateAllEntries();

			UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
			APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
			for (auto& Compensation : Info->CompensationArray) {
				const FUIItemTableInfo* ItemInfo = GameInst->FindUIItemInfo(Compensation.ItemName);
				GameMode->GetMainHUD()->GetInventory()->AddItem(ItemInfo, Compensation.CompensationMount);
			}
		}
	}
	
}
