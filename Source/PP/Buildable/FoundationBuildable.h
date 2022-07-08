// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "FoundationBuildable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EFoundationSocket : uint8
{
	BuildComponents,
	FoundationSockets,
	FoundationSocketUp,
	FoundationSocketDown,
	FoundationSocketRight,
	FoundationSocketLeft,
	TriangleFoundationSockets,
	TriangleFoundationSocketUp,
	TriangleFoundationSocketDown,
	TriangleFoundationSocketRight,
	TriangleFoundationSocketLeft,
	WallSockets,
	WallSocketUp,
	WallSocketDown,
	WallSocketRight,
	WallSocketLeft,
	RampSockets,
	RampSocketUp,
	RampSocketDown,
	RampSocketRight,
	RampSocketLeft,
	StairsSocket,
	FloorSocket,
	RoofSocket
};

UCLASS()
class PP_API AFoundationBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	AFoundationBuildable();

private:
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoundationSockets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoundationSocketUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoundationSocketDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoundationSocketRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FoundationSocketLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TriangleFoundationSockets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TriangleFoundationSocketUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TriangleFoundationSocketDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TriangleFoundationSocketRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TriangleFoundationSocketLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* WallSockets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* WallSocketUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* WallSocketDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* WallSocketRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* WallSocketLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RampSockets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RampSocketUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RampSocketDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RampSocketRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RampSocketLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* StairsSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* FloorSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RoofSocket;
*/
protected:
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);

	
};
