// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicCircle.h"


AMagicCircle::AMagicCircle() {

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>	DecalMtrlAsset(TEXT("MaterialInstanceConstant'/Game/Character/Player/MTGreystoneMagicCircle_Inst.MTGreystoneMagicCircle_Inst'"));

	if (DecalMtrlAsset.Succeeded())
		m_Decal->SetDecalMaterial(DecalMtrlAsset.Object);

	m_Material = m_Decal->CreateDynamicMaterialInstance();

	m_Glow = 3.f;
	m_Dir = 1.f;
	
	SetActorScale3D(FVector(0.6f, 0.6f, 0.6f));

}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	m_Glow += m_Dir * DeltaTime * 5.f;

	if (m_Glow >= 15.f)
	{
		m_Glow = 15.f;
		m_Dir = -1.f;
	}

	else if (m_Glow <= 3.f)
	{
		m_Glow = 3.f;
		m_Dir = 1.f;
	}

	m_Material->SetScalarParameterValue(TEXT("Glow"), m_Glow);
	//m_Material->SetVectorParameterValue()
	//m_Material->Parameter

}
