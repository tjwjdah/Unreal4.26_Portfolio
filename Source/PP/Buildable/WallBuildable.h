// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "WallBuildable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWallSocket : uint8
{
	BuildComponents,
	WallSocket,
	CeilingSockets,
	CeilingSocketUp,
	CeilingSocketDown,
	TriangleCeilingSockets,
	TriangleCeilingSocketUp,
	TriangleCeilingSocketDown,
	RoofSockets,
	RoofSocketUp,
	RoofSocketsDown
};

UCLASS()
class PP_API AWallBuildable : public ABuildable
{
	GENERATED_BODY()
public:
		AWallBuildable();

protected:
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);
	
};
