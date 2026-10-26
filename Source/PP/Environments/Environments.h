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
		EnvironmentsType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString DropInfoString;

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

	UStaticMesh* GetMesh() {
		return m_Mesh->GetStaticMesh();
	}

	
	int32 GetHP() {
		return m_Hp;
	}
	
	EnvironmentsType GetType() { return Type; }
	FString GetDropInfoString(){return DropInfoString;}
	virtual void Attacked();

};
