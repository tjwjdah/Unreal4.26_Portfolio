// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ABossTrigger.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerDelegate);

UCLASS()
class PP_API AABossTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABossTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UShapeComponent* m_Trigger;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ETriggerType	m_ShapeType;
		*/
	UPROPERTY(BlueprintAssignable, Category = "Collision")
		FTriggerDelegate	m_TriggerBeginDelegate;


	UPROPERTY(BlueprintAssignable, Category = "Collision")
		FTriggerDelegate	m_TriggerEndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ULevelSequence* m_SequenceAsset;

	ULevelSequencePlayer* m_SequencePlayer;
	ALevelSequenceActor* m_SequenceActor;


	bool			m_Destroy;
	float			m_Time;
public:
	FTriggerDelegate& GetBeginDelegate()
	{
		return m_TriggerBeginDelegate;
	}

	FTriggerDelegate& GetEndDelegate()
	{
		return m_TriggerEndDelegate;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void TriggerBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void TriggerBegin();

	UFUNCTION()
		void TriggerEnd();
};
