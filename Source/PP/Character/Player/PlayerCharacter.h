// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Character.h"
#include "Components/SceneCaptureComponent2D.h"
#include "../../Equipment/UseItem.h"
#include "../../Widget/UseInventoryTile.h"

#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE(FNameDelegate);
UCLASS()
class PP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	
	
	bool			   m_BuildableCheck;

	class UPlayerAnim* m_AnimInstance;
	
	int32  BuildRotAmount;

	USkeletalMesh* m_PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_HeadMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* m_GarmentMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* m_GarmentSkeletalMesh;
	
	bool m_GarmentOnOff;
	int  m_GarmentArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator Control_Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector  InPut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		bool Stuuned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		bool Sprinting;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		float JogSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		float SprintSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Building)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Building)
		float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Building)
		class UBuildManagerComponent* BuildManager;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class AWeapon* m_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Weapon)
		bool m_WeaponOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Weapon)
		bool m_Aiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* m_ArrowSpawnLocation;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_BowMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAnimMontage*	m_AxeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAnimMontage*	m_PickMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AUseItem*> m_UseInven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_UseInvenNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_AttackMontageArray;
	FNameDelegate	m_NameDelegate;


	int32 m_Hp;
	int32 m_PreHp;

	float m_Engrgy;
	float m_PreEngrgy;


	float m_Hunger;
	float m_PreHunger;
	float m_HungerTick;

	float m_Thirst;
	float m_PreThirst;
	float m_ThirstTick;

	class  UCharacterState* m_StateBar;
	class  UMainHUD* m_MainHUD;

	bool m_OnSkill2;

	class AEffectDecal* m_Decal;

	bool m_SwordAttackEnable;
	int32 m_SwordAttackIndex;
	
	bool  m_EatItemOn;

	int32 m_BuildUpgradeNum;

	class ABuildable* m_CheckedBuild;
	const FUIItemTableInfo* m_BuildUpgradeInfo;


	bool m_Start;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);



public:
	void MoveForwardKey(float Scale);
	void MoveSideKey(float Scale);
	void TurnKey(float Scale);
	void LookUpKey(float Scale);
	

	void BuildRot();

	void JumpKey();
	void AttackKey();
	void AttackReKey();

	void RequestBuild();
	void ToggleBuildMode();
	
	void SprintKeyPress();
	void SprintKeyRelease();
	void WeaponOnOffKey();
	void Inventory();

	void OneKey();
	void TwoKey();
	void ThreeKey();
	void fourKey();
	void fiveKey();
	void sixKey();
public:
	UFUNCTION(BlueprintCallable)
	void Sprint(bool _Sprinting);
	void CanSprint();

	void ItemUse(int32 _UseNum);

	void Test();

	void WeapomSetSocket(const FString& Path);

	AWeapon* GetWeapon() { return m_Weapon; }
	void SetWeapon(AWeapon* _Weapon);
	
	void SetCrosshair();

	void SetInvenItem(AUseItem* _Item, int32 _Num);

	//void GetInvenItem(int32 _Num);
	FTransform ArrowTransform();

	class UBuildManagerComponent* GetBuildManager();

	FRotator GetControl_Rotation() { return Control_Rotation; }

	void SetWeaponOn(bool _WeaponOn) { m_WeaponOn = _WeaponOn; }

	class UPlayerAnim* GetPlayerAnim();

	void NormalAttack();
	template <typename T>
	void SetDelegate(T* Obj, void(T::* Func)())
	{
		m_NameDelegate.BindUObject(Obj, Func);
	}

	void DelegateUnBind() {
		if(m_NameDelegate.IsBound())
		m_NameDelegate.Unbind();
	}

	void Action() {
		if (m_NameDelegate.IsBound())
			m_NameDelegate.Execute();
	}
	void SetStateBar(class UCharacterState* _StateBar);
	
	
	void PlayerCheckBeginOverlap();

	void PlayerCheckEndOverlap();

	void Test22();

	
	void SwordAttackEnd();
	void EnableSwordAttack(bool Enable)
	{
		m_SwordAttackEnable = Enable;
	}

	void SetEatItemOn(bool _OnOff) { m_EatItemOn = _OnOff; }

	void SetGarmentMesh(const FUIItemTableInfo* _Info);

	bool GetGarmentOnOff() {return m_GarmentOnOff;}

	UFUNCTION()
		void Upgrade();

	void PlayerStart() { m_Start = true; }
};
