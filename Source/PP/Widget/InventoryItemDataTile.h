// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDataTile.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UInventoryItemDataTile : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItemDataTile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D* m_IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_UsePossible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_MaxAMount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_AMount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool UseOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FLinearColor m_Color;
public:
	void SetIconTexture(UTexture2D* IconTex)
	{
		m_IconTexture = IconTex;
	}

	void SetNameText(const FString& NameText)
	{
		m_NameText = NameText;
	}
	void SetIndex(int32 Index)
	{
		m_Index = Index;
	}
	UTexture2D* GetIconTexture()
	{
		return m_IconTexture;
	}

	FString GetNameText()
	{
		return m_NameText;
	}

	int32 GetIndex()
	{
		return m_Index;
	}
	bool GetUsePossible()
	{
		return m_UsePossible;
	}
	void SetUsePossible(bool UsePossible)
	{
		m_UsePossible = UsePossible;
	}
	int32 GetItemIndex()
	{
		return m_ItemIndex;
	}
	void SetItemIndex(int32 ItemIndex)
	{
		m_ItemIndex = ItemIndex;
	}

	int32 GetAMount()
	{
		return m_AMount;
	}
	int32 GetMaxAMount()
	{
		return m_MaxAMount;
	}

	void SetAMount(int32 AMount)
	{
		m_AMount = AMount;
	}
	void SetMaxAMount(int32 MaxAMount)
	{
		m_MaxAMount = MaxAMount;
	}
	bool IsFull()
	{
		if (m_AMount == m_MaxAMount)
			return true;
		else
			return false;
	}
	void SetUse(bool _Use) { UseOn = _Use; }
	bool GetUse() { return UseOn; }

	void SetColor(FLinearColor _Color) { m_Color = _Color; }
	FLinearColor GetColor() { return m_Color; }
};
