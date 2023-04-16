// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "MonsterAnim.h"
#include "GameFramework/Character.h"
#include "../../Tools/ItemBox.h"
#include "Monster.generated.h"


USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32		Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FMonsterAttackInfo> AttackInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FDropItem>  DropItem;

};

UCLASS()
class PP_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FMonsterInfo	m_MonsterInfo;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* m_HPBar;


	class UHPBar* m_HPBarWidget;
	*/
	AActor* m_SpawnPoint;



	class UMonsterAnim* m_AnimInstance;
	FString			m_MonsterInfoName;

	bool		m_AttackEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FVector>	m_PatrolArray;
	int32			m_PatrolIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool		m_DissolveEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool        m_NotSummon;

	bool		m_SummonStart;

	float		m_Dissolve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float		m_DissolveTime;



	FVector     m_DissolveColor;
	FVector		m_SummonColor;

	float		m_PatrolLength;
	float		m_CurrentPatrolLength;
	int32		m_CurrentPatrolIndex;

	float		m_DissolveAccTime;
	float		m_DissolveRange;



	bool		m_PatrolEnable;
	
	class APatrolPointSpline* m_PatrolSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector	m_MoveVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D	m_DestVector;
	//TArray<FString>	m_DropItemNameArray;


	AActor* m_AttackPlayer;

	float				m_BeAttackedEndtime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				m_Wait;

	bool				m_HitOn;
	float				m_HitTimeAcc;
	float				m_HitTime;
	float				m_Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf <AItemBox> m_DropItem;

	bool					m_ItemBoxProduceOn;


	bool				m_DeathOn;
	bool                m_QuestCheck;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FMonsterInfo& GetMonsterInfo()
	{
		return m_MonsterInfo;
	}

	void ChangeAnimType(EMonsterAnimType Type);

	bool GetAttackEnd()	const
	{
		return m_AttackEnd;
	}
	void SetAttackEnd(bool AttackEnd)
	{
		m_AttackEnd = AttackEnd;
	}
	void SetPatrolEnable(bool Enable)
	{
		m_PatrolEnable = Enable;
	}
	bool GetPatrolEnable() {
		return m_PatrolEnable;
	}
	bool GetPatrolWait();
	FVector GetPatrolPointSpline();

	void SetPatrolPointSpline(class APatrolPointSpline* Spline)
	{
		m_PatrolSpline = Spline;
	}

	void SetPatrolLength(float Length)
	{
		m_PatrolLength = Length;
	}

	FVector GetPatrolPoint()
	{
		if (m_PatrolIndex >= m_PatrolArray.Num())
			m_PatrolIndex = 0;

		return m_PatrolArray[m_PatrolIndex];
	}

	void NextPatrolPoint()
	{
		++m_PatrolIndex;
		++m_CurrentPatrolIndex;

		if (m_PatrolIndex >= m_PatrolArray.Num())
			m_PatrolIndex = 0;

		if (m_CurrentPatrolIndex >= 3)
			m_CurrentPatrolIndex = 0;
	}
	void SetSpawnPoint(AActor* SpawnPoint);

	AActor* GetSpawnPoint() { return m_SpawnPoint; }

	void AddPatrolPoint(const FVector& Point)
	{
		m_PatrolArray.Add(Point);
	}

	FVector GetMoveLocation() { return m_MoveVector; }

	void Death();


	AActor* GetAttackPlayer() {
		return m_AttackPlayer;
	}
	void SetAttackPlayer(AActor* _Actor) {
		m_AttackPlayer = _Actor;
	}



	bool GetWait() { return m_Wait; }
	void SetWait(bool _Wait) { m_Wait = _Wait; }

	void SetMonsterInfoName(FString _Name) { m_MonsterInfoName = _Name; }

	int32 GetHp() { return m_MonsterInfo.HP; }

	bool GetHit() { return m_HitOn; }
public:
	virtual void NormalAttack();
	virtual void DeathEffect();
	virtual void RandChangeAttack(int32 _First = 0, int32 _End = 0) {

		int32 AttackNum;
		if(_End == 0)
			AttackNum =  FMath::RandRange(_First, m_AnimInstance->GetAttackMax());
		else
			AttackNum = FMath::RandRange(_First, _End);
		m_AnimInstance->SetAttackType((EMosnterAttackType)AttackNum);
	};

	void SetAttackType(int32 _Num)
	{
		if(_Num <= m_AnimInstance->GetAttackMax())
		m_AnimInstance->SetAttackType((EMosnterAttackType)_Num);
	}
	int32 GetAttackType() { return (int32)m_AnimInstance->GetAttackType(); }

	
};
