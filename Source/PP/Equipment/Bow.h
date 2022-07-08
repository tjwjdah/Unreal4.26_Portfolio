// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Weapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class PP_API ABow : public AWeapon
{
	GENERATED_BODY()
public:
	ABow();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* m_SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UAnimInstance>			m_AnimInstance;

	

public:
	virtual void SetMesh(USkeletalMesh* _Mesh);
	
	void ArrowVisibility(bool _bool);
	void ArrowShot(float _AimAlpha, FTransform _Transform);
	virtual void Use();


};
