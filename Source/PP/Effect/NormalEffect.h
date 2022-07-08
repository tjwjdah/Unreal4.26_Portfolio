// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "NormalEffect.generated.h"

UCLASS()
class PP_API ANormalEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalEffect();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_ParticleSystem;

	FSoftObjectPath		m_AsyncParticlePath;
	FSoftObjectPath		m_AsyncSoundPath;

	TSharedPtr<FStreamableHandle>	m_AsyncParticleLoadHandle;
	TSharedPtr<FStreamableHandle>	m_AsyncSoundLoadHandle;

	bool	m_AssetLoop;

public:
	void AssetLoop()
	{
		m_AssetLoop = true;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void LoadParticle(const FString& Path);
	void LoadParticleAsync(const FString& Name);
	void LoadParticleAsyncComplete();
	void LoadSound(const FString& Path);
	void LoadSoundAsync(const FString& Name);
	void LoadSoundAsyncComplete();

public:
	UFUNCTION()
		void ParticleFinish(UParticleSystemComponent* Particle);

	UFUNCTION()
		void SetParticleVisible(bool Visible) {
		m_ParticleSystem->SetVisibility(Visible);
	}

	
};
