// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"

#include "Buildable.generated.h"

UCLASS()
class PP_API ABuildable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USceneComponent*> SceneComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UDestructibleComponent* m_DestructibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_StaticMesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	//USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	UBoxComponent* CollisionVolume;

	FVector					ActorSize;
	FVector					ResultLocation;
	FRotator				ResultRotator;
	int32					HitAmount;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	//UAnimationAsset* BuildAnimation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
	//UAnimationAsset* PreviuewDestroyAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UMaterialInterface*>		BaseMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* CheckSucceededMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* CheckFailMaterial;
	bool BuildCheck;
	bool PreBuildCheck;
	bool FirstCheck;
	int32                   Durability;
	int32                   DurabilityMax;
	float					m_DestroyTime;
	float					m_DestroyTimeMax;

	EBuildType       m_BuildType;
	EnvironmentsType m_MaterialType;

	bool Destruct;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<ABuildable*> Pillars;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		TArray<ABuildable*> Ceilings;
protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable,Category = Building)
	virtual void Build();

	virtual void BuildChecking(FVector Location);

	bool GetBuildCheck() { return BuildCheck; }

	USceneComponent* GetSceneComponent(int32 _SocketNum) {
		if (SceneComponents.Num() > _SocketNum)	return SceneComponents[_SocketNum];
		else
			return nullptr;}
	void SetDurability(int32 _Durability) {Durability = _Durability;}
	void SetMaxDurability(int32 _Durability) { DurabilityMax = _Durability; }
	int32 GetDurability() { return Durability; }
	int32 GetMaxDurability(){ return DurabilityMax; }
	void SetMesh(UDestructibleMesh* _Destructiblemesh, UStaticMesh* _Staticmesh);
	void SetMaterialType(EnvironmentsType _MaterialType) { m_MaterialType = _MaterialType; }
	EnvironmentsType GetMaterialType() { return m_MaterialType; }

	void SetBuildType(EBuildType _BuildType) { m_BuildType = _BuildType; }
	EBuildType GetBuildType() { return m_BuildType; }
	virtual void BuildDestroy();


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


	virtual void Pillardelete(ABuildable* _Pillar);
	virtual void Ceilingsdelete(ABuildable* _Pillar);
	virtual void AddPillar(ABuildable* _Pillar);

	virtual void AddCeilings(ABuildable* _Ceiling);


};
