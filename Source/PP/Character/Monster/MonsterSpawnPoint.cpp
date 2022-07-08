// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"

#include "PatrolPoint.h"
#include "PatrolPointSpline.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Monster = nullptr;

	m_SpawnTime = 1.f;
	m_AccTime = 0.f;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	m_AccTime = m_SpawnTime;

	//if (m_MonsterClass)
	//{
	//	FActorSpawnParameters	param;
	//	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//	AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_MonsterClass,
	//		GetActorLocation(), GetActorRotation(), param);

	//	Monster->SetSpawnPoint(this);

	//	Monster->AddPatrolPoint(GetActorLocation());

	//	const TArray<FVector>& PatrolPointArray = m_PatrolPointSpline->GetPointArray();

	//	for (auto& Point : PatrolPointArray)
	//	{
	//		LOG(TEXT("Point : %s"), *Point.ToString());

	//		Monster->AddPatrolPoint(Point);
	//	}

	//	/*for (auto& Point : m_PatrolPointArray)
	//	{
	//		Monster->AddPatrolPoint(Point->GetActorLocation());
	//	}*/

	//	m_Monster = Monster;
	//}
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!m_Monster)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= m_SpawnTime)
		{
			m_AccTime = 0.f;

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMonster* Monster = GetWorld()->SpawnActor<AMonster>(m_MonsterClass,
				GetActorLocation(), GetActorRotation(), param);
			
			Monster->SetSpawnPoint(this);

			if (m_PatrolPointSpline)
			{
				Monster->SetPatrolPointSpline(m_PatrolPointSpline);
				Monster->SetPatrolLength(m_PatrolPointSpline->GetSplineLength());

				//Monster->AddPatrolPoint(GetActorLocation());

				const TArray<FVector>& PatrolPointArray = m_PatrolPointSpline->GetPointArray();

				for (auto& Point : PatrolPointArray)
				{
					Monster->AddPatrolPoint(Point);
				}
			}

			else
			{
				Monster->AddPatrolPoint(GetActorLocation());

				for (auto& Point : m_PatrolPointArray)
				{
					Monster->AddPatrolPoint(Point->GetActorLocation());
				}
			}


			m_Monster = Monster;
		}
	}
}


