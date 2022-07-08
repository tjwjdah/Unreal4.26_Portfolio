// Fill out your copyright notice in the Description page of Project Settings.


#include "Zoobie.h"

AZoobie::AZoobie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/HighQualityZombie01/HighQualityZombie01.HighQualityZombie01'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Character/Monster/BPZoobieAnim.BPZoobieAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("Zombie");

	m_Attack = false;
	m_SummonColor = FVector(0.f, 0.f, 0.f);
	m_Summon = false;
}

// Called when the game starts or when spawned
void AZoobie::BeginPlay()
{
	GetMesh()->CreateDynamicMaterialInstance(0);
	if (!m_Summon)
		GetMesh()->CreateDynamicMaterialInstance(1);
	Super::BeginPlay();

}

// Called every frame
void AZoobie::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	
	
}

void AZoobie::NormalAttack()
{
	
	FVector	MinionLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	result;
	bool Sweep = GetWorld()->SweepSingleByChannel(result, MinionLoc,
		MinionLoc + Forward * m_MonsterInfo.AttackInfos[GetAttackType()].AttackDistance, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(30.f),
		params);
	/*
#if ENABLE_DRAW_DEBUG

	FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

	FVector Center = MinionLoc + Forward * m_MonsterInfo.AttackInfos[GetAttackType()].AttackDistance / 2.f;

	DrawDebugCapsule(GetWorld(), Center, m_MonsterInfo.AttackInfos[GetAttackType()].AttackDistance / 2.f,
		30.f, FRotationMatrix::MakeFromZ(Forward).ToQuat(),
		DrawColor, false, 0.5f);

#endif
*/
	if (Sweep)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		
		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage((float)m_MonsterInfo.Attack * m_MonsterInfo.AttackInfos[GetAttackType()].Attackdiameter, DmgEvent, GetController(), this);
	}
}

void AZoobie::Summon()
{
	
	m_NotSummon = false;
	m_DissolveEnable = true;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), m_DissolveEnable);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("NotSummon"), -1.f);
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("DissolveColor"), m_SummonColor);
}

float AZoobie::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	return Damage;
}