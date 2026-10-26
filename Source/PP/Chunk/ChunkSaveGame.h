// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/SaveGame.h"
#include "ChunkManager.h"
#include "ChunkSaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBuildingSaveData
{
    GENERATED_BODY()

        UPROPERTY(SaveGame)
        int32 ID = INDEX_NONE;

    UPROPERTY(SaveGame)
        FString BuildDefinitionName;

    UPROPERTY(SaveGame)
        FTransform Transform;

    UPROPERTY(SaveGame)
        int32 Durability = 0;

    UPROPERTY(SaveGame)
        TMap<FName, int32> SocketMap;
};

USTRUCT(BlueprintType)
struct FResourceSaveData
{
    GENERATED_BODY()

        UPROPERTY(SaveGame)
        int32 ID = INDEX_NONE;

    UPROPERTY(SaveGame)
        FTransform Transform;

    UPROPERTY(SaveGame)
        int32 Hp = 0;

    UPROPERTY(SaveGame)
        EnvironmentsType ResourceType = EnvironmentsType::Wood;

    UPROPERTY(SaveGame)
        FString ResourceDefinitionName;

};

USTRUCT(BlueprintType)
struct FAttachBuildableSaveData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    int32 ParentBuildingID = INDEX_NONE;

    UPROPERTY(SaveGame)
        FString BuildDefinitionName;

    UPROPERTY(SaveGame)
        FName ParentSocketName;

    UPROPERTY(SaveGame)
        FTransform Transform;

    UPROPERTY(SaveGame)
        int32 Durability = INDEX_NONE;
    // UPROPERTY(SaveGame)
    // bool bOpened = false;
};

USTRUCT(BlueprintType)
struct FChunkSaveData
{
    GENERATED_BODY()

        UPROPERTY(SaveGame)
        FChunkCoords Coords;

    UPROPERTY(SaveGame)
        TArray<FBuildingSaveData> Buildings;

    UPROPERTY(SaveGame)
        TArray<FResourceSaveData> Resources;

    UPROPERTY(SaveGame)
        TArray<FAttachBuildableSaveData> AttachBuildables;
};

UCLASS()
class PP_API UChunkSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
        FString LevelName;

    UPROPERTY(SaveGame)
        int32 SaveVersion = 1;

    UPROPERTY(SaveGame)
        int32 NextBuildingID = 0;

    UPROPERTY(SaveGame)
        int32 NextResourceID = 1000000;

    UPROPERTY(SaveGame)
        TArray<FChunkSaveData> Chunks;

};
