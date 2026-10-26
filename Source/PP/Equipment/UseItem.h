// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"

#include "UseItem.generated.h"

UCLASS()
class PP_API UUseItem : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UUseItem();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* m_Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType m_ItemType;
protected:
	// Called when the game starts or when spawned
	

public:	
	// Called every frame
	EItemType GetItemType() {return m_ItemType; }
	void SetItemType(EItemType _Type) { m_ItemType = _Type; }
	void SetPlayerOwner(class APlayerCharacter* _Owner);
	virtual void Use() {};
};
