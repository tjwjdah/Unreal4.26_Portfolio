// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "BuildManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PP_API UBuildManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	UFUNCTION(BlueprintCallable)
	void ToggleBuildMode();
	UFUNCTION(BlueprintCallable)
	bool RequestBuild();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
		FVector GridSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
		float BuildDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
		float FloorHeight;

	
	
private:
	bool IsBuilding;

	class APlayerCharacter* m_Player;
	UCameraComponent* PlayerCamera;
	USpringArmComponent* PlayerArm;

	class ABuildable* CurrentBuild;

	EBuildType       m_BuildType;
	TArray<class ABuildable*> BuildiObject;

	UDestructibleMesh* DestructibleMesh;
	UStaticMesh*		 StaticMesh;

	int32                    Durability;
	int32                    MaxDurability;
	EnvironmentsType		MaterialType;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector GetMousePickingLocation();
	FVector GetGridLocation(FVector MouseLocation);
	FRotator GetNextBuildRotation();
	

public:
	class ABuildable* GetCurrentBuild();
	bool GetBuilding() { return IsBuilding; }
	void SetBuildType(EBuildType _BuildType) { m_BuildType = _BuildType; }
	void SetDurability(int32 _Durability) { Durability = _Durability; }
	void SetMaxDurability(int32 _Durability) { Durability = _Durability; }
	void SetMesh(UDestructibleMesh* _DestructibleMesh, UStaticMesh* _StaticMesh) {
		DestructibleMesh = _DestructibleMesh;  StaticMesh = _StaticMesh;
	}

	void SetMaterialType(EnvironmentsType _MaterialType) { MaterialType = _MaterialType; }
	void SetPlayer(APlayerCharacter* _Player);
};
