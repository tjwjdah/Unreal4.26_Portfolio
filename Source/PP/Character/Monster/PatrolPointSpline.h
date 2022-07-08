// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "PatrolPointSpline.generated.h"

UCLASS()
class PP_API APatrolPointSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPointSpline();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USplineComponent* m_Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_DivideCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_MeshVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* m_DebugMesh;

	float	m_SplineLength;
	float	m_CellDistance;

	int32	m_PrevDivide;
	float	m_PrevLength;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FVector>	m_PointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FRotator>	m_RotatorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*>	m_MeshArray;

public:
	const TArray<FVector>& GetPointArray()
	{
		return m_PointArray;
	}

	float GetSplineLength()	const
	{
		return m_Spline->GetSplineLength();
	}

	FVector GetSplinePoint(float Distance)	const
	{
		return m_Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	}

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
		void ComputeSpline();

};
