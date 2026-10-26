// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "BuildAssetSubsystem.generated.h"

/**
 * 
 */
class ABuildable;
class UStaticMesh;
class UDestructibleMesh;

USTRUCT(BlueprintType)
struct FBuildAssets
{
	GENERATED_BODY()

		UPROPERTY(Transient)
		TSubclassOf<ABuildable> BuildClass = nullptr;

	UPROPERTY(Transient)
		UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(Transient)
		UDestructibleMesh* DestructibleMesh = nullptr;


	bool IsValid() { return BuildClass && StaticMesh && DestructibleMesh; }
};


DECLARE_DELEGATE_TwoParams(FBuildAssetsLoad, bool, FBuildAssets);

UCLASS()
class PP_API UBuildAssetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TMap<FString, FBuildAssets> Cache;

	TMap<FString, TSharedPtr<FStreamableHandle>> Handles;

public:
	const FBuildAssets* FindCached(const FString& Name);

	bool ResolveSync(const FString& Name, FBuildAssets& Assets);
	void RequestAsync(const FString& Name, FBuildAssetsLoad Callback);

	void ClearCached(const FString& Name);

	bool BuildLoadList(const FString& Name, const FBuildDefinitionInfo*& Info, TArray<FSoftObjectPath>& Paths);
	bool ResolveLoad(const FString& Name, FBuildAssets& Assets);



};
