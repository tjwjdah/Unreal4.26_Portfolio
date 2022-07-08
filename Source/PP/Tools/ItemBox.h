// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"


USTRUCT(BlueprintType)
struct FBoxItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	aMount;
};

UCLASS()
class PP_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_Mesh;

	// Drop Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FBoxItems>		m_ItemNames;

	class UItemBoxAnim* m_ItemBoxAnim;

	float	    m_DestroyTime;
	bool		m_DestroyOn;
	bool		m_PlayerOn;

	bool		m_BoxOn;
public:
	void AddItem(FBoxItems Items)
	{
		m_ItemNames.Add(Items);
	}

	void SetMesh(USkeletalMesh* Mesh)
	{
		m_Mesh->SetSkeletalMesh(Mesh);
	}

	void ItemBoxClose();
	void ItemBoxOpen();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION()
		void ItemBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void ItemEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void LootItemAddCallback();

};
