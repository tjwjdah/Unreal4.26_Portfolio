// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Tools/ItemBox.h"
#include "Actor.h"
#include "Environments.generated.h"
/**
 * 
 */
UCLASS()
class PP_API AEnvironments : public AActor
{
	GENERATED_BODY()

public:
	AEnvironments();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf <AItemBox> m_DropItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EnvironmentsType m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FDropItem> m_DropItems;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void SetMesh(UStaticMesh* Mesh)
	{
		m_Mesh->SetStaticMesh(Mesh);
	}

	virtual void Attacked();

	EnvironmentsType GetEnvironmentsType() { return m_Type; }
};
