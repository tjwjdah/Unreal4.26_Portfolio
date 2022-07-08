// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAnim.h"
#include "../../PPGameInstance.h"
#include "MonsterAIController.h"
#include "MonsterSpawnPoint.h"
#include "PatrolPointSpline.h"
#include "../Player/PlayerCharacter.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//m_HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetCanBeDamaged(true);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	static ConstructorHelpers::FClassFinder<AItemBox>	ItemBox(TEXT("Blueprint'/Game/Item/DropItem/BP_Box.BP_Box_C'"));
	if (ItemBox.Succeeded())
		m_DropItem = ItemBox.Class;

	m_Dissolve = 1.5f;
	m_DissolveTime = 5.f;
	m_DissolveAccTime = 0.f;
	m_DissolveEnable = false;
	m_DissolveMin = -1.f;
	m_DissolveMax = 1.5f;

	m_DissolveRange = m_DissolveMax - m_DissolveMin;

	m_DissolveColor = FVector(1.0f, 0.f, 0.f);

	m_PatrolEnable = false;
	m_PatrolLength = 0.f;
	m_CurrentPatrolLength = 0.f;
	m_CurrentPatrolIndex = 0;
	
	m_NotSummon = true;
	m_Wait = false;

	m_HitOn = false;
	m_HitTime = 0.75f;
	m_HitTimeAcc = 0.f;
	m_ItemBoxProduceOn = false;
	m_DeathOn = false;
	m_SummonStart = false;

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{



	Super::BeginPlay();
	m_AnimInstance = Cast<UMonsterAnim>(GetMesh()->GetAnimInstance());


	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FMonsterTableInfo* Info = GameInst->FindMonsterInfo(m_MonsterInfoName);

		if (Info)
		{
			m_MonsterInfo.Name = Info->Name;
			m_MonsterInfo.Attack = Info->Attack;
			m_MonsterInfo.Armor = Info->Armor;
			m_MonsterInfo.HP = Info->HP;
			m_MonsterInfo.HPMax = Info->HPMax;
			m_MonsterInfo.MP = Info->MP;
			m_MonsterInfo.MPMax = Info->MPMax;
			m_MonsterInfo.Level = Info->Level;
			m_MonsterInfo.Exp = Info->Exp;
			m_MonsterInfo.Gold = Info->Gold;
			for (int32 i = 0; i < Info->AttackInfos.Num(); i++)
				m_MonsterInfo.AttackInfos.Add(Info->AttackInfos[i]);
			m_MonsterInfo.MoveSpeed = Info->MoveSpeed;
			m_MonsterInfo.TraceDistance = Info->TraceDistance;
			m_MonsterInfo.MoveDistance = Info->MoveDistance;
			m_MonsterInfo.DropItem = Info->DropItem;
		}

	}

	//GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), 1.f);
	
	m_AnimInstance = Cast<UMonsterAnim>(GetMesh()->GetAnimInstance());
	m_DissolveRange = m_DissolveMax - m_DissolveMin;
	m_Dissolve = m_DissolveMax;

	m_AttackEnd = false;
	m_BeAttackedEndtime = 0.f;
	


	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*
	if (m_AttackPlayer !=nullptr)
	{
		if(m_NotSummon)
		m_BeAttackedEndtime += DeltaTime;
		
		if (m_BeAttackedEndtime > 5.f)
		{
			m_AttackPlayer = nullptr;
			m_BeAttackedEndtime = 0.f;
		}
	}
	*/
	if (m_PatrolEnable)
	{
		
	}
	m_MoveVector = GetActorLocation();
	
	FVector2D Mlo = FVector2D(m_DestVector.X - m_MoveVector.X, m_DestVector.Y - m_MoveVector.Y);
	Mlo.Normalize();
	m_MoveVector.X += Mlo.X * DeltaTime * 20000.f;
	m_MoveVector.Y += Mlo.Y * DeltaTime * 20000.f;
	Mlo = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	if (FVector2D::Distance(m_DestVector, Mlo) < 200.f)
	{
		m_PatrolEnable = false;
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), FVector2D::Distance(m_DestVector, FVector2D((GetActorLocation().X, GetActorLocation().Y)))));
		m_DestVector.X = m_SpawnPoint->GetActorLocation().X + (FMath::FRandRange(-1.f, 1.f) * m_MonsterInfo.MoveDistance);
		m_DestVector.Y = m_SpawnPoint->GetActorLocation().Y + (FMath::FRandRange(-1.f, 1.f) * m_MonsterInfo.MoveDistance);
	}

	if (m_DissolveEnable)
	{
		if (m_NotSummon)
		{
			m_DissolveAccTime += DeltaTime;

			m_Dissolve = m_DissolveMax - (m_DissolveAccTime / m_DissolveTime * m_DissolveRange);

			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve"), m_Dissolve);

			if (m_DissolveAccTime >= m_DissolveTime)
			{
				m_DissolveAccTime = m_DissolveTime;

				m_Dissolve = m_DissolveMin;

				DeathEffect();

				Destroy();
			}
		}
		else{
			m_DissolveAccTime += DeltaTime;


			if (m_DissolveAccTime > 0.2f && m_SummonStart==false)
			{
				GetMesh()->SetVisibility(true);
				m_SummonStart = true;
			}
			m_Dissolve = m_DissolveMax - (m_DissolveAccTime / m_DissolveTime * m_DissolveRange);

			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve"), m_Dissolve);

			if (m_DissolveAccTime >= m_DissolveTime)
			{
		

				m_Wait = false;
				m_NotSummon = true;
				m_DissolveEnable = false;
				m_DissolveRange = m_DissolveMax - m_DissolveMin;
				m_Dissolve = m_DissolveMax;
				m_DissolveAccTime = 0.f;
				GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), m_DissolveEnable);
				GetMesh()->SetScalarParameterValueOnMaterials(TEXT("NotSummon"), m_NotSummon);
				GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve"), m_Dissolve);

				
			}
		}
	}

	if (m_HitOn)
	{
		m_HitTimeAcc += DeltaTime;
		m_Hit = (1.f- (m_HitTimeAcc/ m_HitTime))*5.f;
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Hit"), m_Hit);
		if (m_HitTimeAcc >= m_HitTime)
		{
			m_HitTimeAcc = 0.f;
			m_Hit = 0.f;
			m_HitOn = false;
			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitOn"), -1.f);
		}
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMonster::ChangeAnimType(EMonsterAnimType Type)
{
	m_AnimInstance->ChangeAnimType(Type);
}

void AMonster::NormalAttack()
{
}

void AMonster::DeathEffect()
{
}

float AMonster::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage %f"), Damage));
	if (Damage == 0.f)
		return 0.f;


	m_HitOn = true;
	m_HitTimeAcc = 0.f;
	m_AttackPlayer =  DamageCauser;
	
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitOn"), 1.f);
	m_BeAttackedEndtime = 0.f;
	Damage = Damage - m_MonsterInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;


	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage %f"), Damage));
	m_MonsterInfo.HP -= Damage;
	
	// 몬스터가 죽었을 경우
	if (m_MonsterInfo.HP <= 0)
	{
		m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

		AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		if (MonsterController)
			MonsterController->BrainComponent->StopLogic(TEXT("Dead"));
		
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (m_MonsterInfo.DropItem.Num() > 0 && !m_DeathOn)
		{
			m_DeathOn = true;
			TArray<FBoxItems> Items;
			for (int32 i = 0; i < m_MonsterInfo.DropItem.Num(); i++)
			{
				int32 aMount = FMath::RandRange(m_MonsterInfo.DropItem[i].MinaMount, m_MonsterInfo.DropItem[i].MaxaMount);
				if (aMount > 0)
				{
					FBoxItems Item;
					Item.aMount = aMount;
					Item.ItemName = m_MonsterInfo.DropItem[i].ItemName;
					Items.Add(Item);
					m_ItemBoxProduceOn = true;
				}
			}
			
			if (m_ItemBoxProduceOn)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AItemBox* ItemBox = GetWorld()->SpawnActor<AItemBox>(m_DropItem->GetDefaultObject()->GetClass(),
					GetActorLocation() + FVector(0.f, 0.f, 50.f),
					GetActorRotation(), param);
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("????")));
				for (int32 i = 0; i < Items.Num(); i++)
					ItemBox->AddItem(Items[i]);

				
			}
		}

		Death();
		
	}

	return Damage;
}

void AMonster::SetSpawnPoint(AActor* SpawnPoint)
{
	m_SpawnPoint = SpawnPoint;
	m_DestVector.X = m_SpawnPoint->GetActorLocation().X + (FMath::FRandRange(-1.f, 1.f) * m_MonsterInfo.MoveDistance);
	m_DestVector.Y = m_SpawnPoint->GetActorLocation().Y + (FMath::FRandRange(-1.f, 1.f) * m_MonsterInfo.MoveDistance);
}

void AMonster::Death()
{
	// 죽었을때 처리할 내용을 처리한다.
	// 죽었을때의 효과나 아이템 생성 등을 처리한다.
	// 객체를 제거한다.
	//Destroy();

	m_DissolveEnable = true;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), m_DissolveEnable);
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("DissolveColor"), m_DissolveColor);
	
	
}


bool AMonster::GetPatrolWait()
{
	if (m_PatrolSpline)
		return m_PatrolSpline->GetSplineLength() / 3.f * (m_CurrentPatrolIndex + 1) <= m_CurrentPatrolLength;
	else return false;
}

FVector AMonster::GetPatrolPointSpline()
{
	if (m_PatrolSpline)
		return m_PatrolSpline->GetSplinePoint(m_CurrentPatrolLength);
	else 
		return FVector::ZeroVector;
}
