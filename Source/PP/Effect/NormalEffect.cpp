// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEffect.h"
// Sets default values
ANormalEffect::ANormalEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	SetRootComponent(m_ParticleSystem);

	m_AssetLoop = false;

	
}

// Called when the game starts or when spawned
void ANormalEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_AssetLoop)
		m_ParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish);
}

void ANormalEffect::LoadParticle(const FString& Path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), *Path);

	if (Particle)
		m_ParticleSystem->SetTemplate(Particle);
}

void ANormalEffect::LoadParticleAsync(const FString& Name)
{
}

void ANormalEffect::LoadParticleAsyncComplete()
{
}

void ANormalEffect::LoadSound(const FString& Path)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);

	if (Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
}

void ANormalEffect::LoadSoundAsync(const FString& Name)
{
	
}

void ANormalEffect::LoadSoundAsyncComplete()
{
	m_AsyncSoundLoadHandle->ReleaseHandle();

	TAssetPtr<USoundBase>	NewSound(m_AsyncSoundPath);

	if (NewSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NewSound.Get(), GetActorLocation());
}

void ANormalEffect::ParticleFinish(UParticleSystemComponent* Particle)
{
	Destroy();
}

