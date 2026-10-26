// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "SpawnItem.h"
#include "SpawnWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ASpawnWeapon : public ASpawnItem
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASpawnWeapon();

protected:
	EWeaponType m_Type;

	FString     m_SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			m_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			m_BuildAttack;

public:
	void SetAttack(int32 _Attack) { m_Attack = _Attack; }
	void SetBuildAttack(int32 _Attack) { m_BuildAttack = _Attack; }
	int32 GetAttack() { return m_Attack; }
	int32 GetBuildAttack() { return m_BuildAttack; }
	void SetWeaponType(EWeaponType Type) { m_Type = Type; }
	EWeaponType GetWeaponType() { return m_Type; }
};
