// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "EffectDecal.generated.h"

UCLASS()
class PP_API AEffectDecal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffectDecal();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDecalComponent* m_Decal;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
