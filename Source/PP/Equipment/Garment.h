// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Equipment.h"
#include "Garment.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AGarment : public AEquipment
{
	GENERATED_BODY()
public:
	AGarment();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32			m_Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* m_SkeletalMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetMesh(USkeletalMesh* _Mesh) { m_SkeletalMesh = _Mesh; }

	void SetArmor(int32 _Armor) { m_Armor = _Armor; }

	virtual void Use();
};
