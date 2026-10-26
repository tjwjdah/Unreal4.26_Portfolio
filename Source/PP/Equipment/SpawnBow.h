// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "SpawnWeapon.h"
#include "SpawnBow.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ASpawnBow : public ASpawnWeapon
{
	GENERATED_BODY()

		ASpawnBow();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAnimInstance>			m_AnimInstance;

public:
	virtual void BeginPlay() override;

	void ArrowVisibility(bool _bool);
	void ArrowShot(float _AimAlpha, FTransform _Transform);

};
