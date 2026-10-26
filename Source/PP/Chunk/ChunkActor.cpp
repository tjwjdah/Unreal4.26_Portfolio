// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkActor.h"

// Sets default values
AChunkActor::AChunkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
}

// Called when the game starts or when spawned
void AChunkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChunkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AChunkActor::AddInstance(UStaticMesh* Mesh, const FTransform& Transform,int32 ID)
{
	EObType Type = (ID < 1000000) ? EObType::Building : EObType::Resource;
	UHierarchicalInstancedStaticMeshComponent* HISM = GetAddHISM(Mesh, Type);

	if (!HISM)return -1;

	FTransform Trans = Transform.GetRelativeTransform(GetActorTransform());
	int32 Index= HISM->AddInstance(Trans);
	
	TMap<UStaticMesh*, FIIMap>* RegistryMap = nullptr;
	if (Type == EObType::Building)
		RegistryMap = &BuildingRegistryMap;
	else
		RegistryMap = &ResourceRegistryMap;

	if (!RegistryMap->Contains(Mesh))
	{
		RegistryMap->Add(Mesh, FIIMap());
	}

	(*RegistryMap)[Mesh].Map.Add(Index, ID);
	return Index;
}


int32 AChunkActor::RemoveInstance(UStaticMesh* Mesh, int32 InstanceIndex, EObType Type)
{
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*>* HISMCompMap = nullptr;
	TMap<UStaticMesh*, FIIMap>* RegistryMap = nullptr;
	if (Type == EObType::Building) {
		HISMCompMap = &BuildingHISMCompMap;
		RegistryMap = &BuildingRegistryMap;
	}
	else {
		HISMCompMap = &ResourceHISMCompMap;
		RegistryMap = &ResourceRegistryMap;
	}
	if (!HISMCompMap->Contains(Mesh)) return-1;
	if (!RegistryMap->Contains(Mesh)) return -1;
	UHierarchicalInstancedStaticMeshComponent* HISM = (*HISMCompMap)[Mesh];
	TMap<int32, int32>& M = (*RegistryMap)[Mesh].Map;

	if (!HISM->RemoveInstance(InstanceIndex) )return-1;

	if (M.Contains(InstanceIndex))
		M.Remove(InstanceIndex);

	int32 LastIndex = HISM->GetInstanceCount();

	if (InstanceIndex == LastIndex) return -1;

	if (M.Contains(LastIndex)) {
		int32 ID = M[LastIndex];
		M.Remove(LastIndex);
		M.Add(InstanceIndex, ID);
		return ID;
	}

	return -1;
}

int32 AChunkActor::GetMeshID(UInstancedStaticMeshComponent* HISM, int32 Index)
{
	if (!IsValid(HISM))
		return -1;
	for (auto& Pair : BuildingHISMCompMap)
	{
		if (Pair.Value == HISM)
		{
			if (BuildingRegistryMap.Contains(Pair.Key) && BuildingRegistryMap[Pair.Key].Map.Contains(Index))
			{
				return BuildingRegistryMap[Pair.Key].Map[Index];
			}
			return -1;
		}
	}
	for (auto& Pair : ResourceHISMCompMap)
	{
		if (Pair.Value == HISM)
		{
			if (ResourceRegistryMap.Contains(Pair.Key) &&ResourceRegistryMap[Pair.Key].Map.Contains(Index))
			{
				return ResourceRegistryMap[Pair.Key].Map[Index];
			}
			return -1;
		}
	}

	return -1;
}

UHierarchicalInstancedStaticMeshComponent* AChunkActor::GetAddHISM(UStaticMesh* Mesh, EObType Type)
{
	if(!Mesh)
	return nullptr;

	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*>* HISMCompMap = nullptr;
	if (Type == EObType::Building)
		HISMCompMap = &BuildingHISMCompMap;
	else
		HISMCompMap = &ResourceHISMCompMap;

	if (HISMCompMap->Contains(Mesh))
		return (*HISMCompMap)[Mesh];

	UHierarchicalInstancedStaticMeshComponent* NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	NewHISM->RegisterComponent();
	NewHISM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	NewHISM->SetStaticMesh(Mesh);
	if (Type ==EObType::Building)
		NewHISM->SetCollisionProfileName(TEXT("Build"));
	else
		NewHISM->SetCollisionProfileName(TEXT("Environments"));
	
	HISMCompMap->Add(Mesh, NewHISM);

	return NewHISM;
}



UHierarchicalInstancedStaticMeshComponent* AChunkActor::FindHISM(UStaticMesh* Mesh, EObType Type)
{
	if (Type == EObType::Building) {
		if (BuildingHISMCompMap.Contains(Mesh)) {
			return BuildingHISMCompMap[Mesh];
		}
	}
	else {
		if (ResourceHISMCompMap.Contains(Mesh)) {
			return ResourceHISMCompMap[Mesh];
		}
	}

	return nullptr;
}

void AChunkActor::ClearBuildingHISMCompMap()
{
	for (auto HISMComp : BuildingHISMCompMap) {
		HISMComp.Value->ClearInstances();
		HISMComp.Value->DestroyComponent();
	}
	BuildingHISMCompMap.Empty();
	BuildingRegistryMap.Empty();
}

void AChunkActor::ClearResourceHISMCompMap()
{
	for (auto HISMComp : ResourceHISMCompMap) {
		HISMComp.Value->ClearInstances();
		HISMComp.Value->DestroyComponent();
	}
	ResourceHISMCompMap.Empty();
	ResourceRegistryMap.Empty();
}

void AChunkActor::ClearAll()
{
	ClearBuildingHISMCompMap();
	ClearResourceHISMCompMap();
}

bool AChunkActor::HasResourceHISM()
{
	for (auto& Pair : ResourceHISMCompMap)
	{
		if (IsValid(Pair.Value) && Pair.Value->GetInstanceCount() > 0)
			return true;
	}

	return false;
}

int32 AChunkActor::GetBuildingInstanceCount()
{
	int32 Count = 0;

	for (const auto& Pair : BuildingHISMCompMap)
	{
		if (IsValid(Pair.Value))
		{
			Count += Pair.Value->GetInstanceCount();
		}
	}

	return Count;
}

int32 AChunkActor::GetResourceInstanceCount()
{
	int32 Count = 0;

	for (const auto& Pair : ResourceHISMCompMap)
	{
		if (IsValid(Pair.Value))
		{
			Count += Pair.Value->GetInstanceCount();
		}
	}

	return Count;
}

int32 AChunkActor::GetTotalInstanceCount()
{
	return GetBuildingInstanceCount() + GetResourceInstanceCount();
}

int32 AChunkActor::GetHISMComponentCount()
{
	int32 Count = 0;

	for (const auto& Pair : BuildingHISMCompMap)
	{
		if (IsValid(Pair.Value))
		{
			Count++;
		}
	}

	for (const auto& Pair : ResourceHISMCompMap)
	{
		if (IsValid(Pair.Value))
		{
			Count++;
		}
	}

	return Count;
}

