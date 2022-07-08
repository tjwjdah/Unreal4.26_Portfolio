// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPointSpline.h"

// Sets default values
APatrolPointSpline::APatrolPointSpline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	SetRootComponent(m_Spline);

	m_DivideCount = 100;
	m_PrevDivide = 0;
	m_PrevLength = m_Spline->GetSplineLength();

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	DebugMeshAsset(TEXT("StaticMesh'/Game/Etc/Cube_2.Cube_2'"));

	if (DebugMeshAsset.Succeeded())
		m_DebugMesh = DebugMeshAsset.Object;

	m_MeshVisible = true;
}

void APatrolPointSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ComputeSpline();
}

// Called when the game starts or when spawned
void APatrolPointSpline::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APatrolPointSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APatrolPointSpline::ComputeSpline()
{
	m_PointArray.RemoveAll([](FVector v) {return true; });
	m_RotatorArray.RemoveAll([](FRotator v) {return true; });

	if (m_MeshVisible)
	{
		if (m_DivideCount != m_PrevDivide)
		{
			for (int32 i = 0; i < m_MeshArray.Num(); ++i)
			{
				if (m_MeshArray[i])
					m_MeshArray[i]->DestroyComponent();
			}

			if (m_MeshArray.Num() > 0)
				m_MeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
		}
	}

	else
	{
		for (int32 i = 0; i < m_MeshArray.Num(); ++i)
		{
			if (m_MeshArray[i])
				m_MeshArray[i]->DestroyComponent();
		}

		if (m_MeshArray.Num() > 0)
			m_MeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
	}

	m_SplineLength = m_Spline->GetSplineLength();

	m_CellDistance = m_SplineLength / m_DivideCount;

	LOG(TEXT("Spline Divide : %d"), m_DivideCount);

	FTransform Transform = GetActorTransform();

	bool	MeshEmpty = m_MeshArray.Num() == 0;

	for (int32 i = 0; i <= m_DivideCount; ++i)
	{
		FVector Point = m_Spline->GetLocationAtDistanceAlongSpline(m_CellDistance * i,
			ESplineCoordinateSpace::World);

		m_PointArray.Add(Point);

		FVector LocalPoint = Transform.InverseTransformPosition(Point);

		FRotator Rot = m_Spline->GetRotationAtDistanceAlongSpline(m_CellDistance * i,
			ESplineCoordinateSpace::World);

		m_RotatorArray.Add(Rot);

		if (m_MeshVisible)
		{
			if (m_DivideCount != m_PrevDivide || MeshEmpty)
			{
				UStaticMeshComponent* MeshCom = NewObject<UStaticMeshComponent>(this);

				FTransform MeshTransform = MeshCom->GetRelativeTransform();

				MeshTransform.SetLocation(LocalPoint);
				MeshTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

				MeshCom->SetRelativeTransform(MeshTransform);

				MeshCom->SetWorldRotation(Rot);

				MeshCom->SetStaticMesh(m_DebugMesh);

				m_MeshArray.Add(MeshCom);

				MeshCom->SetupAttachment(m_Spline);
			}

			else
			{
				m_MeshArray[i]->SetRelativeLocation(LocalPoint);
				m_MeshArray[i]->SetWorldRotation(Rot);
			}
		}
	}

	m_PrevDivide = m_DivideCount;
	m_PrevLength = m_SplineLength;
}
