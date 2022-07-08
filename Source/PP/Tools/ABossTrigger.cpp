// Fill out your copyright notice in the Description page of Project Settings.


#include "ABossTrigger.h"

// Sets default values
AABossTrigger::AABossTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

	SetRootComponent(m_Trigger);

	m_SequenceAsset = nullptr;
	m_SequencePlayer = nullptr;
}

// Called when the game starts or when spawned
void AABossTrigger::BeginPlay()
{
	Super::BeginPlay();

	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABossTrigger::TriggerBeginOverlap);
	m_Trigger->OnComponentEndOverlap.AddDynamic(this, &AABossTrigger::TriggerEndOverlap);
	
	m_TriggerBeginDelegate.AddDynamic(this, &AABossTrigger::TriggerBegin);
	m_TriggerEndDelegate.AddDynamic(this, &AABossTrigger::TriggerEnd);

	m_Destroy = false;
}

// Called every frame
void AABossTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (m_Destroy)
	{
		m_Time += DeltaTime;
		if (m_Time > 4.5)
		{
			m_Time = 0.f;
			Destroy();
		}
	}
}

void AABossTrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_TriggerBeginDelegate.Broadcast();
}

void AABossTrigger::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_TriggerEndDelegate.Broadcast();
}

void AABossTrigger::TriggerBegin()
{
	if (m_SequenceAsset)
	{
		// 만약 시퀀스 플레이어가 없다면 생성해준다.
		if (!m_SequencePlayer)
		{
			m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(), m_SequenceAsset, FMovieSceneSequencePlaybackSettings(),
				m_SequenceActor);
		}

		m_SequencePlayer->Play();

		//PrintViewport(1.f, FColor::Red, TEXT("SequencePlay"));

		m_Destroy = true;
	}

}

void AABossTrigger::TriggerEnd()
{

}
