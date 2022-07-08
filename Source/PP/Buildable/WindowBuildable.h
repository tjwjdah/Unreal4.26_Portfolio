// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "SetWindowBuildable.h"
#include "WindowBuildable.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWindowSocket : uint8
{
	BuildComponents,
	WallSocket,
	CeilingSocket1,
	CeilingSocket2,
	TriangleCeilingSocket1,
	TriangleCeilingSocket2,
	RoofSocket1,
	RoofSocket2,
	WindowSocket
};
UCLASS()
class PP_API AWindowBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	AWindowBuildable();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UBoxComponent* CheckPropCollision;
	

	

protected:
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);

	

};
