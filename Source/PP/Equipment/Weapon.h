// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AWeapon : public AUseItem
{
	GENERATED_BODY()

public:
	AWeapon();
protected:
	EWeaponType m_Type;

	FString     m_SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			m_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			m_BuildAttack;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EWeaponType GetWeaponType() { return m_Type; }

public:
	virtual void SetMesh(const FString& Path) {};
	void SetAttack(int32 _Attack) { m_Attack = _Attack; }
	void SetBuildAttack(int32 _Attack) { m_BuildAttack = _Attack; }
	int32 GetAttack() { return m_Attack; }
	int32 GetBuildAttack() { return m_BuildAttack; }
};

