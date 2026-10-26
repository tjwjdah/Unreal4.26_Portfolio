// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Chunk/ChunkManager.h"
#include "../AssetManager/BuildAssetSubsystem.h"
#include "Components/ActorComponent.h"
#include "BuildManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PP_API UBuildManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildManagerComponent();

	

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector GridSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float BuildDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float FloorHeight;
	bool IsBuilding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* m_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* PlayerArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABuildable* CurrentBuild;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString BuildDefinitionName;

	const FBuildDefinitionInfo* BuildDefinitionInfo = nullptr;

	UPROPERTY(Transient)
		FBuildAssets CurrentAssets;

	bool bPreviewAssetsReady = false;

	bool bOverlapCheck;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	FVector GetMousePickingLocation();
	FVector GetGridLocation(FVector MouseLocation);
	FRotator GetNextBuildRotation();
	

public:
	class ABuildable* GetCurrentBuild();
	bool GetBuilding() { return IsBuilding; }
	
	void SetBuildData(FString Name);

	void BuilingDestroy();
	
	void SetPlayer(APlayerCharacter* _Player);

	void Building();
	bool OverlapCheckCanBuild();
	class UStaticMeshSocket* FindClosestSocket(const FBuildingNode& Node, FVector Location);


	UFUNCTION(BlueprintCallable)
		void ToggleBuildMode();
	UFUNCTION(BlueprintCallable)
		bool RequestBuild();
};
