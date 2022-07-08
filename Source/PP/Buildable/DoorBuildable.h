// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Buildable.h"
#include "Components/TimeLineComponent.h"
#include "DoorBuildable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDoorSocket : uint8
{
	BuildComponents,
	WallSocket,
	CeilingSocket1,
	CeilingSocket2,
	TriangleCeilingSocket1,
	TriangleCeilingSocket2,
	RoofSocket1,
	RoofSocket2,
	DoorSocket
};
UCLASS()
class PP_API ADoorBuildable : public ABuildable
{
	GENERATED_BODY()
public:
	ADoorBuildable();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UBoxComponent* DoorCollisionVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UArrowComponent* DoorAxis;
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		class AOpenDoorBuildable* m_Door;

	bool			m_DelegateCheck;

	FTimeline MyTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_Open;
	
	UPROPERTY(EditAnywhere, Category = Timeline)
	UCurveFloat* CurveFloat;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Building)
	virtual void Build(); 

	virtual void BuildChecking(FVector Location);

	
	void SetDoor(class AOpenDoorBuildable* _Door);

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
