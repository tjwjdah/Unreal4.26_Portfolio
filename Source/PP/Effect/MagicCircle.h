// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "EffectDecal.h"
#include "MagicCircle.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AMagicCircle : public AEffectDecal
{
	GENERATED_BODY()

public:
	AMagicCircle();

protected:

	UMaterialInstanceDynamic* m_Material;
	float		m_Glow;
	float		m_Dir;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
