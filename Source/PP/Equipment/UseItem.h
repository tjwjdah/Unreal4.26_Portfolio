// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"

#include "UseItem.generated.h"

UCLASS()
class PP_API AUseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUseItem();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* m_Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType m_ItemType;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	EItemType GetItemType() {return m_ItemType; }
	void SetItemType(EItemType _Type) { m_ItemType = _Type; }
	void SetOwner(class APlayerCharacter* _Owner);
	virtual void Use() {};
};
