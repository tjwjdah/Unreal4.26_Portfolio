// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"

#include "OpenDoorBuildable.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AOpenDoorBuildable : public ABuildable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UBoxComponent* DoorCollisionVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UArrowComponent* DoorAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* DoorSocket;
	
	bool			m_DelegateCheck;

	FTimeline MyTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool m_Open;

	UPROPERTY(EditAnywhere, Category = Timeline)
		UCurveFloat* CurveFloat;
public:
	AOpenDoorBuildable();
protected:

	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build();

//	virtual void BuildChecking(FVector Location);

	virtual void BuildDestroy();


	UFUNCTION()
		void OpenDoor(float Value);

	UFUNCTION()
		void PlayerOpenDoor();

	UFUNCTION()
		void DoorBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void DoorEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
