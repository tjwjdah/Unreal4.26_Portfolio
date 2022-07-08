// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Environments.h"
#include "Tree.generated.h"

UCLASS()
class PP_API ATree : public AEnvironments
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATree();
	
private:
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
