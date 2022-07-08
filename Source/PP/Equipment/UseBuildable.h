// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UseItem.h"
#include "UseBuildable.generated.h"

/**
 * 
 */
UCLASS()
class PP_API AUseBuildable : public AUseItem
{
	GENERATED_BODY()
public:
	AUseBuildable();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UDestructibleMesh* m_DestructibleMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building)
		UStaticMesh* m_StaticMesh;
	int32                   Durability;
	int32                   MaxDurability;
	EBuildType       m_BuildType;
	EnvironmentsType m_MaterialType;
public:
	void SetDurability(int32 _Durability) { Durability = _Durability; }
	void SetMaxDurability(int32 _Durability) { MaxDurability = _Durability; }
	
	void SetMesh(UDestructibleMesh* _DestructibleMesh, UStaticMesh* _StaticMesh) {
		m_DestructibleMesh = _DestructibleMesh; m_StaticMesh = _StaticMesh;}
	void SetBuildType(EBuildType _BuildType) { m_BuildType = _BuildType; }
	void SetMaterialType(EnvironmentsType _MaterialType) { m_MaterialType = _MaterialType; }
	virtual void Use();

};
