// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "EatItem.generated.h"

/**
 * 
 */


UCLASS()
class PP_API UEatItem : public UUseItem
{
	GENERATED_BODY()

public:
	UEatItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FItemOption>	m_ItemOptions;
		
	// Called when the game starts or when spawned

public:
	// Called every frame
	virtual void Use();
	void SetOptions(FItemOption _Option) {
		m_ItemOptions.Add(_Option);
	}
	TArray<FItemOption>& GetOptions() { return m_ItemOptions; }
};
