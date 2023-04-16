// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGameInstance.h"

UPPGameInstance::UPPGameInstance()
{
	
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterInfoTableAsset(TEXT("DataTable'/Game/Character/Monster/etc/DTMonsterInfo.DTMonsterInfo'"));

	if (MonsterInfoTableAsset.Succeeded())
		m_MonsterInfoTable = MonsterInfoTableAsset.Object;
	/*
	static ConstructorHelpers::FObjectFinder<UDataTable>	PlayerInfoTableAsset(TEXT("DataTable'/Game/Player/DTPlayerInfo.DTPlayerInfo'"));

	if (PlayerInfoTableAsset.Succeeded())
		m_PlayerInfoTable = PlayerInfoTableAsset.Object;
		*/
	
	static ConstructorHelpers::FObjectFinder<UDataTable>	UIItemInfoTableAsset(TEXT("DataTable'/Game/Item/DTItemTable.DTItemTable'"));

	if (UIItemInfoTableAsset.Succeeded())
		m_UIItemInfoTable = UIItemInfoTableAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	UIQuestInfoTableAsset(TEXT("DataTable'/Game/Item/DTQuest.DTQuest'"));

	if (UIQuestInfoTableAsset.Succeeded())
		m_UIQuestInfoTable = UIQuestInfoTableAsset.Object;
	/*
	static ConstructorHelpers::FObjectFinder<UDataTable>	QuestInfoTableAsset(TEXT("DataTable'/Game/Quest/DTQuest.DTQuest'"));

	if (QuestInfoTableAsset.Succeeded())
		m_QuestInfoTable = QuestInfoTableAsset.Object;

		*/
}

UPPGameInstance::~UPPGameInstance()
{
	
}

void UPPGameInstance::Init()
{
	Super::Init();

	
	
	
	
	// 각 퀘스트들이 완료가 되었는지 여부를 가져온다.
	FQuestData	Data;

	const FQuestTableInfo* Info = FindUIQuestInfo(TEXT("QHunt1"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	for (auto& CompleteData : Info->InfoArray) {
		FQuestDataInfo DataInfo;
		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count; 
		DataInfo.Count = 0;
		DataInfo.Complete = false;
		Data.CompleteArray.Add(DataInfo);
	}
	
	for (auto& CompleteData : Info->CompensationArray) {
		FQuestCompensation CompensationInfo;
		CompensationInfo.CompensationMount = CompleteData.CompensationMount;
		CompensationInfo.ItemName = CompleteData.ItemName;
		Data.CompensationArray.Add(CompensationInfo);
	}
	
	Data.Complete = false;
	
	m_QuestComplete.Add(TEXT("QHunt1"), Data);


	

	Info = FindUIQuestInfo(TEXT("QCratf1"));
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });
	Data.CompensationArray.RemoveAll([](FQuestCompensation v) {return true; });
	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	for (auto& CompleteData : Info->InfoArray) {
		FQuestDataInfo DataInfo;
		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;
		Data.CompleteArray.Add(DataInfo);
	}
	
	for (auto& CompleteData : Info->CompensationArray) {
		FQuestCompensation CompensationInfo;
		CompensationInfo.CompensationMount = CompleteData.CompensationMount;
		CompensationInfo.ItemName = CompleteData.ItemName;
		Data.CompensationArray.Add(CompensationInfo);
	}
	
	Data.Complete = false;
	m_QuestComplete.Add(TEXT("QCratf1"), Data);
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });
	/*
	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest1"), Data);

	Info = FindQuestInfo(TEXT("Quest2"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });

	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest2"), Data);

	Info = FindQuestInfo(TEXT("Quest3"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });

	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest3"), Data);

	Info = FindQuestInfo(TEXT("Quest4"));

	Data.Name = Info->Name;
	Data.QuestDesc = Info->QuestDesc;
	Data.CompleteArray.RemoveAll([](FQuestDataInfo v) {return true; });

	for (auto& CompleteData : Info->InfoArray)
	{
		FQuestDataInfo	DataInfo;

		DataInfo.Type = CompleteData.Type;
		DataInfo.DestName = CompleteData.DestName;
		DataInfo.MaxCount = CompleteData.Count;
		DataInfo.Count = 0;
		DataInfo.Complete = false;

		Data.CompleteArray.Add(DataInfo);
	}

	Data.Complete = false;

	m_QuestComplete.Add(TEXT("Quest4"), Data);
	*/
}

const FMonsterTableInfo* UPPGameInstance::FindMonsterInfo(const FString& Name)
{
	return m_MonsterInfoTable->FindRow<FMonsterTableInfo>(*Name, "");
}

const FPlayerTableInfo* UPPGameInstance::FindPlayerInfo(const FString& Name)
{
	return m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Name, "");
}

const FUIItemTableInfo* UPPGameInstance::FindUIItemInfo(const FString& Name)
{
	return m_UIItemInfoTable->FindRow<FUIItemTableInfo>(*Name, "");
}

const FQuestTableInfo* UPPGameInstance::FindUIQuestInfo(const FString& Name)
{
	return m_UIQuestInfoTable->FindRow<FQuestTableInfo>(*Name, "");
}
