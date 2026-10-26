// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAssetSubsystem.h"
#include "../PPGameInstance.h"
#include "Engine/AssetManager.h"


const FBuildAssets* UBuildAssetSubsystem::FindCached(const FString& Name)
{
	return Cache.Find(Name);
}

bool UBuildAssetSubsystem::ResolveSync(const FString& Name, FBuildAssets& Assets)
{
	const FBuildAssets* Asset = Cache.Find(Name);
		if (Asset) {
			Assets = *Asset;
			return Assets.IsValid();
		}
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
	if (!Inst)
		return false;
	const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(Name);

	if (!Info)
		return false;

	Assets.BuildClass = Info->BuildableClass.LoadSynchronous();
	Assets.StaticMesh = Info->StaticMesh.LoadSynchronous();
	Assets.DestructibleMesh = Info->DestructibleMesh.LoadSynchronous();

	if (!Assets.IsValid())
		return false;

	Cache.Add(Name, Assets);
	return true;
}

void UBuildAssetSubsystem::RequestAsync(const FString& Name, FBuildAssetsLoad Callback)
{
	const FBuildAssets* Asset = Cache.Find(Name);
	if (Asset)
	{
		Callback.ExecuteIfBound(true, *Asset);
		return;
	}

	if (Handles.Contains(Name)) 
		return;

	const FBuildDefinitionInfo* Info=nullptr;
	TArray<FSoftObjectPath> Paths;

	if (!BuildLoadList(Name, Info, Paths)) {
		Callback.ExecuteIfBound(false, FBuildAssets());
		return;
	}
	TSharedPtr<FStreamableHandle> Handle =
		UAssetManager::GetStreamableManager().RequestAsyncLoad(Paths, FStreamableDelegate::CreateWeakLambda(this, [this, Name, Callback]() {
		FBuildAssets BuildAssets;
		const bool bLoad = ResolveLoad(Name, BuildAssets);
		if (bLoad)
			Cache.Add(Name, BuildAssets);

		Handles.Remove(Name);
		Callback.ExecuteIfBound(bLoad, BuildAssets);
		})
	);

	Handles.Add(Name, Handle);
}

void UBuildAssetSubsystem::ClearCached(const FString& Name)
{
	if (Name.IsEmpty())
	{
		for (auto& Pair : Handles)
		{
			if (Pair.Value.IsValid())
			{
				Pair.Value->CancelHandle();
				Pair.Value->ReleaseHandle();
			}
		}

		Handles.Empty();
		Cache.Empty();
		return;
	}

	Cache.Remove(Name);

	if (TSharedPtr<FStreamableHandle>* HandlePtr = Handles.Find(Name))
	{
		if (HandlePtr->IsValid())
		{
			(*HandlePtr)->CancelHandle();
			(*HandlePtr)->ReleaseHandle();
		}
		Handles.Remove(Name);
	}
}

bool UBuildAssetSubsystem::BuildLoadList(const FString& Name, const FBuildDefinitionInfo*& Info, TArray<FSoftObjectPath>& Paths)
{
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
	if (!Inst)
		return false;
	Info = Inst->FindBuildInfo(Name);
	if(!Info)
	return false;

	Paths.Reset();

	if (!Info->BuildableClass.IsNull())
		Paths.AddUnique(Info->BuildableClass.ToSoftObjectPath());

	if (!Info->StaticMesh.IsNull())
		Paths.AddUnique(Info->StaticMesh.ToSoftObjectPath());

	if (!Info->DestructibleMesh.IsNull())
		Paths.AddUnique(Info->DestructibleMesh.ToSoftObjectPath());

	return Paths.Num() > 0;
}

bool UBuildAssetSubsystem::ResolveLoad(const FString& Name, FBuildAssets& Assets) 
{
	UPPGameInstance* Inst = Cast<UPPGameInstance>(GetGameInstance());
	if (!Inst)
		return false;
	const FBuildDefinitionInfo* Info = Inst->FindBuildInfo(Name);
	if(!Info)
	return false;

	Assets.BuildClass = Info->BuildableClass.Get();
	Assets.StaticMesh = Info->StaticMesh.Get();
	Assets.DestructibleMesh = Info->DestructibleMesh.Get();

	return Assets.IsValid();
}
