// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "UseBuildable.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UUseBuildable : public UUseItem
{
	GENERATED_BODY()
public:
	UUseBuildable();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		FString BuildDefinitionName;
public:
	void SetBuildDefinitionName(FString Name) { BuildDefinitionName = Name; }
	virtual void Use();

};
