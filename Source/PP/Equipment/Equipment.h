// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "Equipment.generated.h"

UCLASS()
class PP_API UEquipment : public UUseItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UEquipment();




protected:
	// Called when the game starts or when spawned
	
public:	

public:
	//virtual void SetMesh(const FString& Path) {};
	virtual void SetMesh(USkeletalMesh* _Mesh) {};
	virtual void SetMesh(UStaticMesh* _Mesh) {};
};
