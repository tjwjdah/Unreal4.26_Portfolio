// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../GameInfo.h"
#include "Equipment.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AArrow : public AEquipment
{
	GENERATED_BODY()

public:
	AArrow();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_BuildDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float m_InitialSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool  m_On;

	float m_Time;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void SetDamage(float _Damage) { m_Damage = _Damage; };
	void SetSpeed(float _Speed);
	void SetBuildDamage(float _Damage) { m_BuildDamage = _Damage; };
public:
	UFUNCTION()
		void ProjectileStop(const FHitResult& result);
protected:
	virtual void StopEvent(const FHitResult& result);
};
