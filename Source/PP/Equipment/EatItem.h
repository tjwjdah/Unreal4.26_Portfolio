// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "EatItem.generated.h"

/**
 * 
 */


UCLASS()
class PP_API AEatItem : public AUseItem
{
	GENERATED_BODY()

public:
	AEatItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FItemOption>	m_ItemOptions;
		
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use();

	void SetOptions(FItemOption _Option) {
		m_ItemOptions.Add(_Option);
	}
	TArray<FItemOption>& GetOptions() { return m_ItemOptions; }
};
