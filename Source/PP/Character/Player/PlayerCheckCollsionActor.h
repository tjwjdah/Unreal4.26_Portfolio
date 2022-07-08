// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "PlayerCheckCollsionActor.generated.h"

UCLASS()
class PP_API APlayerCheckCollsionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCheckCollsionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_CheckCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	APlayerCharacter* m_PlayerClass;

	
	int32			m_BottleNum;
	const FUIItemTableInfo* Bottle;
	const FUIItemTableInfo* WaterBottle;

	//TArray<const FUIItemTableInfo*> 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void PlayerCheckBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void PlayerCheckEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SetPlayer(APlayerCharacter* _Player) { m_PlayerClass = _Player; }

	UFUNCTION()
		void GetWater();
};
