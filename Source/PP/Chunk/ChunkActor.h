// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ChunkActor.generated.h"


USTRUCT(BlueprintType)
struct FIIMap
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, int32> Map;
};
UCLASS()
class PP_API AChunkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> BuildingHISMCompMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> ResourceHISMCompMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<UStaticMesh*, FIIMap> BuildingRegistryMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<UStaticMesh*, FIIMap> ResourceRegistryMap;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	int32 AddInstance(UStaticMesh* Mesh, const FTransform& Transform, int32 ID);

	int32 RemoveInstance(UStaticMesh* Mesh, int32 InstanceIndex, EObType Type);

	int32 GetMeshID(UInstancedStaticMeshComponent* HISM, int32 Index);

	UHierarchicalInstancedStaticMeshComponent* GetAddHISM(UStaticMesh* Mesh, EObType Type);

	UHierarchicalInstancedStaticMeshComponent* FindHISM(UStaticMesh* Mesh, EObType Type);
	void ClearBuildingHISMCompMap();
	void ClearResourceHISMCompMap();

	void ClearAll();

	bool HasResourceHISM();


	int32 GetBuildingInstanceCount();
	int32 GetResourceInstanceCount();
	int32 GetTotalInstanceCount();
	int32 GetHISMComponentCount();
};
