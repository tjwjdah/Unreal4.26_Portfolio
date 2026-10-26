// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGameStateBase.h"

#include "Chunk/ChunkManager.h"

void APPGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	ChunkManager =Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
}

AChunkManager* APPGameStateBase::GetChunkManager()
{
	return ChunkManager;
}
