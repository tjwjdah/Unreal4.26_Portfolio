// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "Equipment.generated.h"

UCLASS()
class PP_API AEquipment : public AUseItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipment();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//virtual void SetMesh(const FString& Path) {};
	virtual void SetMesh(USkeletalMesh* _Mesh) {};
	virtual void SetMesh(UStaticMesh* _Mesh) {};
};
