// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "CeilingBuildable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECeilingSocket : uint8
{
	BuildComponents,
	CeilingSocket1,
	CeilingSocket2,
	CeilingSocket3,
	CeilingSocket4,
	WallSocket1,
	WallSocket2,
	WallSocket3,
	WallSocket4,
	StairsSocket,
	FloorSocket,
	TriangleCeilingSocket1,
	TriangleCeilingSocket2,
	TriangleCeilingSocket3,
	TriangleCeilingSocket4
};

UCLASS()
class PP_API ACeilingBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	ACeilingBuildable();
protected:
	virtual void BeginPlay() override;
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);

};
