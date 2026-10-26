// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "DestructibleBuildable.generated.h"

UCLASS()
class PP_API ADestructibleBuildable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleBuildable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UDestructibleComponent* DestructibleMesh;

	float Time;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDestructibleMesh(UDestructibleMesh* Destructiblemesh);
};
