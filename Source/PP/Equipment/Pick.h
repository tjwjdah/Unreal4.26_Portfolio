// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Weapon.h"
#include "Pick.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UPick : public UWeapon
{
	GENERATED_BODY()
public:
	UPick();
private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	USkeletalMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* m_SkeletalMesh;
public:
	virtual void Use();
	virtual void SetMesh(USkeletalMesh* _Mesh);
};
