// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "RampBuildable.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ARampBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	ARampBuildable();
protected:

	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);
};
