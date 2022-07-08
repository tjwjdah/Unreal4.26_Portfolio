// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"

#include "OpenDoorBuildable.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AOpenDoorBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	AOpenDoorBuildable();
protected:

	virtual void BeginPlay() override;

	
	class ADoorBuildable* FrameReference;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);

	virtual void BuildDestroy();
};
