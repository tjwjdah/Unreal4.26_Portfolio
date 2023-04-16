// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "../../Buildable/Buildable.h"
#include "../../Buildable/BuildManagerComponent.h"
#include "../../Widget/MainHUD.h"
#include "../../Widget/DurabilityBar.h"
#include "PlayerAnim.h"

#include "../../PPGameModeBase.h"
#include "../../PPGameInstance.h"
#include "../../Widget/MainHUD.h"
#include "../../Widget/Crosshair.h"
#include "../../Equipment/Weapon.h"
#include "../../Equipment/Bow.h"
#include "../../Environments/Environments.h"
#include "../../Widget/CharacterState.h"
#include "../../Effect/MagicCircle.h"
#include "../../Equipment/EatItem.h"
#include "../../Effect/HitCameraShake.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	CCMale(TEXT("SkeletalMesh'/Game/CharacterCustomizer/Character_Customizer/Characters/CC_Human/CC_Male.CC_Male'"));

	
	
	
	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (CCMale.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CCMale.Object);
		m_PlayerMesh = CCMale.Object;
	}
	
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	m_GarmentMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GarmentMesh"));
	m_HeadMesh->SetupAttachment(GetMesh());
	m_GarmentMesh->SetupAttachment(GetMesh());
	m_Arm->SetupAttachment(GetMesh());

	m_Arm->TargetArmLength = 900.f;
	
	m_Camera->SetupAttachment(m_Arm);
	
	m_ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowLocation"));
	m_ArrowSpawnLocation->SetupAttachment(GetMesh());


	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));


	SetCanBeDamaged(true);
	
	BuildRotAmount = 0;
	BuildManager = CreateDefaultSubobject<UBuildManagerComponent>(TEXT("BuildManager"));
	Stuuned = false;

	JogSpeed = 360.f;
	SprintSpeed =600.f;

	m_WeaponOn = false;

	m_Aiming = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DrawBow(TEXT("AnimMontage'/Game/DynamicCombatSystem/AnimRetargeting/AR_DrawBowMontage.AR_DrawBowMontage'"));

	if (DrawBow.Succeeded())
		m_BowMontageArray.Add(DrawBow.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DisarmBow(TEXT("AnimMontage'/Game/DynamicCombatSystem/AnimRetargeting/Ar_DisarmBow_Montage.Ar_DisarmBow_Montage'"));

	if (DisarmBow.Succeeded())
		m_BowMontageArray.Add(DisarmBow.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	ShootAndDrawBow(TEXT("AnimMontage'/Game/DynamicCombatSystem/AnimRetargeting/AR_ShootAndDrawMontage.AR_ShootAndDrawMontage'"));

	if (ShootAndDrawBow.Succeeded())
		m_BowMontageArray.Add(ShootAndDrawBow.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	AxeAct(TEXT("AnimMontage'/Game/EasyBuildingSystem/Mannequin/AnimationsRetarget/AM_Mannequin_Axe_Act.AM_Mannequin_Axe_Act'"));

	if (AxeAct.Succeeded())
		m_AxeMontage = AxeAct.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	PickAct(TEXT("AnimMontage'/Game/EasyBuildingSystem/Mannequin/AnimationsRetarget/AM_Mannequin_PickAxe_Act.AM_Mannequin_PickAxe_Act'"));

	if (PickAct.Succeeded())
		m_PickMontage = PickAct.Object;
		
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	SwordAttack1(TEXT("AnimMontage'/Game/CharacterCustomizer/Tools/Tool_1H_Weapon/1H_Attack_A_Montage.1H_Attack_A_Montage'"));

	if (SwordAttack1.Succeeded())
		m_AttackMontageArray.Add(SwordAttack1.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	SwordAttack2(TEXT("AnimMontage'/Game/CharacterCustomizer/Tools/Tool_1H_Weapon/1H_Attack_B_Montage.1H_Attack_B_Montage'"));

	if (SwordAttack2.Succeeded())
		m_AttackMontageArray.Add(SwordAttack2.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	SwordAttack3(TEXT("AnimMontage'/Game/CharacterCustomizer/Tools/Tool_1H_Weapon/1H_Attack_C_Montage.1H_Attack_C_Montage'"));

	if (SwordAttack3.Succeeded())
		m_AttackMontageArray.Add(SwordAttack3.Object);


	for (int32 i = 0; i < 6; i++)
	{
		m_UseInven.Add(nullptr);
	}
	m_UseInvenNum = -1;

	m_Hp = 100;
	m_Engrgy = 100.f;
	m_Hunger = 100.f;
	m_Thirst = 100.f;
	m_PreHp = 100;
	m_PreEngrgy = 100.f;
	m_PreHunger = 100.f;
	m_PreThirst = 100.f;
	
	m_BuildableCheck = false;
	m_SwordAttackEnable = true;

	m_SwordAttackIndex = 0;


	GetMesh()->SetReceivesDecals(false);
	m_CheckedBuild = nullptr;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	
	if (m_AnimInstance)
	{
		
	}
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	m_StateBar = GameMode->GetMainHUD()->GetCharacterState();
	m_MainHUD = GameMode->GetMainHUD();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_Start)
		return;
	

	if (m_Aiming)
	{
		SetCrosshair();
	}
	if(Sprinting || m_Aiming)
	{ 
		m_Engrgy -= DeltaTime*2.f;
		if (m_Engrgy <= 0.f && m_Aiming)
		{
			m_Engrgy = 0.f;
			AttackReKey();
		}
	}
	else {
		m_Engrgy += DeltaTime*4;
		if (m_Engrgy > 100.f)
			m_Engrgy = 100.f;
	}
	
	m_HungerTick += DeltaTime;
	m_ThirstTick += DeltaTime;


	if (m_HungerTick > 8.f)
	{
		m_Hunger -= 2.f;
		if (m_Hunger <= 0.f)
		{
			m_Hunger = 0.f;
			m_Hp -= 5.f;
		}
		m_HungerTick = 0.f;
	}
	if (m_ThirstTick > 8.f)
	{
		m_Thirst -= 1.f;
		if (m_Thirst <= 0.f)
		{
			m_Thirst = 0.f;
			m_Hp -= 5.f;
		}
		m_ThirstTick = 0.f;
	}

	if (m_Hp != m_PreHp)
	{
		m_PreHp = m_Hp;
		m_StateBar->SetHealthPercent(m_Hp/100.f);
	}
	if (m_Engrgy != m_PreEngrgy)
	{
		m_PreEngrgy = m_Engrgy;
		m_StateBar->SetEnergyPercent(m_Engrgy / 100.f);
	}
	if (m_Hunger != m_PreHunger)
	{
		m_PreHunger = m_Hunger;
		m_StateBar->SetHungerPercent(m_Hunger / 100.f);
	}
	if (m_Thirst != m_PreThirst)
	{
		m_PreThirst = m_Thirst;
		m_StateBar->SetThirstPercent(m_Thirst / 100.f);
	}
	
	if (m_BuildableCheck)
	{
		FCollisionQueryParams	params(NAME_None, false, GetOwner());
		params.bReturnPhysicalMaterial = true;

		FHitResult	result;
		FVector End = m_Camera->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(m_Camera->GetComponentRotation()) * 1000);

		bool Hit = GetWorld()->LineTraceSingleByChannel(result, m_Camera->GetComponentLocation(), End,
			ECollisionChannel::ECC_GameTraceChannel11, params);

		if (Hit)
		{
			if (result.Actor->IsA(ABuildable::StaticClass()))
			{
				
				ABuildable* Act = Cast<ABuildable>(result.Actor);
				
				m_MainHUD->GetDurabilityBar()->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Act->GetDurability(), Act->GetMaxDurability())));
				m_MainHUD->GetDurabilityBar()->SetDurabilityPercent((float)Act->GetDurability() / (float)Act->GetMaxDurability());

				if (m_CheckedBuild != Act)
				{
					m_CheckedBuild = Act;
					EnvironmentsType MaterialType = Act->GetMaterialType();

					FString BuildName;
					if (MaterialType != EnvironmentsType::Metal)
					{

						switch (Act->GetBuildType())
						{
						case EBuildType::Foundation:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneFoundation");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalFoundation");
							}
						}
						break;
						case EBuildType::Wall:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneWall");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalWall");
							}
						}
						break;
						case EBuildType::Ceiling:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneCeiling");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalCeiling");
							}
						}
						break;
						case EBuildType::DoorFrame:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneDoorFrame");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalDoorFrame");
							}
						}
						break;
						case EBuildType::WindowFrame:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneWindowFrame");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalWindowFrame");
							}
						}
						break;
						case EBuildType::Ramp:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneRamp");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalRamp");
							}
						}
						break;
						case EBuildType::Stairs:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneStairs");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalStairs");
							}
						}
						break;
						case EBuildType::Fence:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneFence");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalFence");
							}
						}
						break;
						case EBuildType::Door:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneDoor");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalDoor");
							}
						}
						break;
						case EBuildType::Window:
						{
							if (MaterialType == EnvironmentsType::Wood)
							{
								BuildName = TEXT("StoneWindow");
							}
							else if (MaterialType == EnvironmentsType::Stone)
							{
								BuildName = TEXT("MetalWindow");
							}
						}
						break;
						}
					
					UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
					m_BuildUpgradeInfo = GameInst->FindUIItemInfo(BuildName);
					m_BuildUpgradeNum = m_MainHUD->GetInventory()->FindItem(m_BuildUpgradeInfo);

					if (m_BuildUpgradeNum != -1)
					{
						m_MainHUD->GetHotKey()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						m_MainHUD->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Upgrade"))));
						m_NameDelegate.BindUObject(this, &APlayerCharacter::Upgrade);
					}
					}
				}
			}
		}
	}
	if (m_OnSkill2)
	{
		FHitResult	result;

		bool Hit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(
			ECollisionChannel::ECC_GameTraceChannel1,
			false, result);

		if (Hit)
		{
			m_Decal->SetActorLocation(result.ImpactPoint);
		}
	}
	if (m_AnimInstance->IsSwordAttack())
	{
		AddMovementInput(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, GetControlRotation().Yaw), 0.05f);
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,
		&APlayerCharacter::MoveForwardKey);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,
		&APlayerCharacter::MoveSideKey);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this,
		&APlayerCharacter::TurnKey);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,
		&APlayerCharacter::LookUpKey);
	
	
	
	PlayerInputComponent->BindAction(TEXT("BuildRot"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::BuildRot);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::AttackKey);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this,
		&APlayerCharacter::AttackReKey);
	PlayerInputComponent->BindAction(TEXT("ToggleBuildMode"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::ToggleBuildMode);
	PlayerInputComponent->BindAction(TEXT("RequestBuild"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::RequestBuild);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Repeat, this,
		&APlayerCharacter::SprintKeyPress);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this,
		&APlayerCharacter::SprintKeyRelease);

	
	PlayerInputComponent->BindAction(TEXT("WeaponOnOff"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::WeaponOnOffKey);


	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Inventory);
	
	PlayerInputComponent->BindAction(TEXT("Quest"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Quest);
	PlayerInputComponent->BindAction(TEXT("One"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::OneKey);
	PlayerInputComponent->BindAction(TEXT("Two"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::TwoKey);
	PlayerInputComponent->BindAction(TEXT("Three"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::ThreeKey);
	PlayerInputComponent->BindAction(TEXT("four"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::fourKey);
	PlayerInputComponent->BindAction(TEXT("five"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::fiveKey);
	PlayerInputComponent->BindAction(TEXT("six"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::sixKey);

	PlayerInputComponent->BindAction(TEXT("KeyAction"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Action);


	PlayerInputComponent->BindAction(TEXT("Test"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Test22);
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage == 0.f)
		return 0.f;
	
	Damage = Damage -  m_GarmentArmor;
	Damage = Damage < 1.f ? 1.f : Damage;

	m_Hp -= Damage;
	if (!m_AnimInstance->GetHit())
	{
		FVector VAngle = DamageCauser->GetActorLocation() - GetActorLocation();
		VAngle.Normalize();
		float FAngle = GetActorRotation().Yaw - FMath::RadiansToDegrees(VAngle.HeadingAngle());

		FAngle *= -1.f;
		if (FAngle < 0.f)
		{
			FAngle = 360 - (FAngle * -1.f);
		}
		m_AnimInstance->SetHitDirection(FAngle);
		m_AnimInstance->SetHit(true);
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), FAngle));
	}
	//GetActorRotation().Yaw
	// 플레이어가 죽었을 경우
	if (m_Hp <= 0)
	{
		//m_AnimInstance->SetDeath(true);

		//AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		//if (MonsterController)
		//	MonsterController->BrainComponent->StopLogic(TEXT("Dead"));
	}

	
	if (IsValid(m_MainHUD))
	{
		m_StateBar->SetHealthPercent(m_Hp / 100.f);
	}

	
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
		UHitCameraShake::StaticClass());
		

	return Damage;
	
}

void APlayerCharacter::MoveForwardKey(float Scale)
{
	float CurveValue = m_AnimInstance->GetCurveValue(TEXT("DisableMovement"));
	if(CurveValue ==0.f && !Stuuned)
		AddMovementInput(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, GetControlRotation().Yaw), Scale);
		
}

void APlayerCharacter::MoveSideKey(float Scale)
{
	float CurveValue = m_AnimInstance->GetCurveValue(TEXT("DisableMovement"));
	if (CurveValue == 0.f && !Stuuned)
		AddMovementInput(FVector(GetActorRightVector().X, GetActorRightVector().Y, GetControlRotation().Yaw), Scale);
		
}


void APlayerCharacter::TurnKey(float Scale)
{

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), Scale));
	AddControllerYawInput(Scale * 40.f * GetWorld()->DeltaTimeSeconds);
	//
}

void APlayerCharacter::LookUpKey(float Scale)
{
	AddControllerPitchInput(Scale * 40.f * GetWorld()->DeltaTimeSeconds);

}
void APlayerCharacter::BuildRot()
{
	if (BuildManager->GetCurrentBuild())
	{
		BuildRotAmount++;
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), BuildRotAmount));
		BuildManager->GetCurrentBuild()->SetActorRotation(FRotator(0.f, 90.f * BuildRotAmount, 0.f));
	}
}

void APlayerCharacter::JumpKey()
{
	Jump();
}

void APlayerCharacter::AttackKey()
{
	if (m_Weapon)
	{
		if (!m_WeaponOn)
		{
			WeaponOnOffKey();
		}
		else
		{
			if(m_Engrgy>10.f)
			switch (m_Weapon->GetWeaponType())
			{
			case EWeaponType::Sword:
			{
				if (m_SwordAttackEnable)
				{
					m_SwordAttackEnable = false;

					if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_SwordAttackIndex]))
					{
						m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_SwordAttackIndex], 0.f);
						m_AnimInstance->Montage_Play(m_AttackMontageArray[m_SwordAttackIndex]);
						m_AnimInstance->SetSwordAttackEnable(true);

						m_SwordAttackIndex = (m_SwordAttackIndex + 1) % m_AttackMontageArray.Num();
					}
				}
			}
				break;
			case EWeaponType::Bow:
			{
				if (!m_AnimInstance->Montage_IsPlaying(m_BowMontageArray[2]))
				{
					
					UCrosshair* Cross = m_MainHUD->GetCrossHair();
					Cross->SetVisibility(ESlateVisibility::Visible);
					m_Aiming = true;
					m_AnimInstance->SetAiming(m_Aiming);

				}
			}
				break;
			case EWeaponType::Pick:
				if (!m_AnimInstance->Montage_IsPlaying(m_PickMontage))
				{
					m_AnimInstance->Montage_SetPosition(m_PickMontage, 0.f);
					m_AnimInstance->Montage_Play(m_PickMontage);

				}
				break;
			case EWeaponType::Axe:
				if (!m_AnimInstance->Montage_IsPlaying(m_AxeMontage))
				{
					m_AnimInstance->Montage_SetPosition(m_AxeMontage, 0.f);
					m_AnimInstance->Montage_Play(m_AxeMontage);

				}
				break;
			case EWeaponType::None:
				break;
			}
			
		
		}

	}
	else if (m_EatItemOn && m_UseInvenNum>=0)
	{
		AEatItem* EatItem = Cast<AEatItem>(m_UseInven[m_UseInvenNum]);
		TArray<FItemOption>& Options = EatItem->GetOptions();
		for (int32 i = 0; i < Options.Num(); i++)
		{
			switch (Options[i].OptionType)
			{
			case EItemOption::HPRecovery:
			{
				m_Hp = m_Hp + Options[i].Option;
				if (m_Hp > 100)
					m_Hp = 100;
				m_StateBar->SetHealthPercent(m_Hp / 100.f);
			}
				break;
			case EItemOption::EnergyRecovery:
			{
				m_Engrgy = m_Engrgy+ Options[i].Option;
				if (m_Engrgy > 100.f)
					m_Engrgy = 100.f;
				m_StateBar->SetEnergyPercent(m_Hp / 100.f);
			}
				break;
			case EItemOption::HungerRecovery:
			{
				m_Hunger = m_Hunger+ Options[i].Option;
				if (m_Hunger > 100.f)
					m_Hunger = 100.f;
				m_StateBar->SetHungerPercent(m_Hp / 100.f);
			}
				break;
			case EItemOption::ThirstRecovery:
			{
				m_Thirst = m_Thirst+ Options[i].Option;
				if (m_Thirst > 100.f)
					m_Thirst = 100.f;
				m_StateBar->SetThirstPercent(m_Hp / 100.f);
			}
				break;
			}
		}
	

		m_MainHUD->GetUseInventory()->ItemUse(m_UseInvenNum);
	}
}
void APlayerCharacter::AttackReKey()
{
	if (m_Weapon)
	{
		switch (m_Weapon->GetWeaponType())
		{
		case EWeaponType::Sword:
			break;
		case EWeaponType::Bow:
		{
			if (m_WeaponOn)
			{
				m_Aiming = false;
			
				UCrosshair* Cross = m_MainHUD->GetCrossHair();
				Cross->SetVisibility(ESlateVisibility::Hidden);
				m_AnimInstance->SetAiming(m_Aiming);
				m_AnimInstance->SetRyo(0.f);
				if (m_AnimInstance->GetBowAimAlpha() >= 1.f)
				{
					m_AnimInstance->Montage_SetPosition(m_BowMontageArray[2], 0.f);
					m_AnimInstance->Montage_Play(m_BowMontageArray[2]);
					ABow* Bow = Cast<ABow>(m_Weapon);
					if (Bow)
					{
						Bow->ArrowShot(m_AnimInstance->GetBowAimAlpha(), ArrowTransform());
						m_Engrgy -= 10.f;
					}
				}
			}
		}
			break;
		case EWeaponType::Pick:
			break;
		case EWeaponType::Axe:
			break;
		case EWeaponType::None:
			break;
		}

	}
}
void APlayerCharacter::RequestBuild()
{
	if (BuildManager->RequestBuild())
	{
		if (m_UseInvenNum >= 0 && m_UseInvenNum < m_UseInven.Num() && m_UseInven[m_UseInvenNum])
		{
	

			m_MainHUD->GetUseInventory()->ItemUse(m_UseInvenNum);
		}
	}
}

void APlayerCharacter::ToggleBuildMode()
{
	if (m_UseInvenNum>=0 && m_UseInvenNum < m_UseInven.Num() && m_UseInven[m_UseInvenNum])
	{
		if (m_UseInven[m_UseInvenNum]->GetItemType() == EItemType::BuildItem) {
				BuildManager->ToggleBuildMode();

			if (BuildManager->GetBuilding() == false)
				BuildRotAmount = 0;
		}
	}
}

void APlayerCharacter::SprintKeyPress()
{
	CanSprint();
	if(Sprinting)
		Sprint(Sprinting);
}

void APlayerCharacter::SprintKeyRelease()
{
	Sprint(false);
}

void APlayerCharacter::Sprint(bool _Sprinting)
{
	Sprinting = _Sprinting;

	if (_Sprinting)
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void APlayerCharacter::CanSprint()
{
	FRotator ARo = FRotator(GetMesh()->GetSocketRotation(TEXT("root")).Roll, 
		GetMesh()->GetSocketRotation(TEXT("root")).Pitch, GetMesh()->GetSocketRotation(TEXT("root")).Yaw - 90.f);
	FVector Ve = GetVelocity();
	
	FRotator BRo = Ve.Rotation();
	FRotator CRo = GetActorRotation();
	bool a;
	if (UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(GetVelocity().Rotation(), CRo).Yaw) < 50.f)
		a = true;
	else
		a = false;

	bool b ;
	if (UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(BRo, CRo).Yaw) < 50.f)
		b = true;
	else
		b = false;
	bool c;
	if (UKismetMathLibrary::BooleanNOR(m_AnimInstance->IsAnyMontagePlaying(), GetCharacterMovement()->IsFalling()))
		c = true;
	else
		c = false;

	if (a && b && c  && m_Engrgy>1.f)
		Sprinting = true;
	else
		Sprinting = false;
	
}


void APlayerCharacter::WeaponOnOffKey() {
	if (m_Weapon)
	{
		switch (m_Weapon->GetWeaponType())
		{
		case EWeaponType::Sword:
			break;
		case EWeaponType::Bow:
		{
			if (m_WeaponOn)
			{
				if (!m_AnimInstance->Montage_IsPlaying(m_BowMontageArray[1]))
				{
					m_AnimInstance->Montage_SetPosition(m_BowMontageArray[1], 0.f);
					m_AnimInstance->Montage_Play(m_BowMontageArray[1]);


					m_WeaponOn = false;
					m_AnimInstance->SetWeaponOnOff(m_WeaponOn);
				}
			}
			else
			{
				if (!m_AnimInstance->Montage_IsPlaying(m_BowMontageArray[0]))
				{
					m_AnimInstance->Montage_SetPosition(m_BowMontageArray[0], 0.f);
					m_AnimInstance->Montage_Play(m_BowMontageArray[0]);

					m_WeaponOn = true;
					m_AnimInstance->SetWeaponOnOff(m_WeaponOn);
				}

			}
		}
			break;
		case EWeaponType::Pick:
			break;
		case EWeaponType::Axe:
			break;
		case EWeaponType::None:
			break;
		}
	}
}

void APlayerCharacter::Inventory()
{

	if (m_MainHUD->GetInventory()->GetVisibility() == ESlateVisibility::Collapsed)
	{
		m_MainHUD->GetInventory()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_MainHUD->GetCrafting()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_MainHUD->GetSelectedBlueprintInfo()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_MainHUD->UI_OnOff(true);
	}
	else
	{
		m_MainHUD->GetInventory()->SetVisibility(ESlateVisibility::Collapsed);
		m_MainHUD->GetCrafting()->SetVisibility(ESlateVisibility::Collapsed);
		m_MainHUD->GetSelectedBlueprintInfo()->SetVisibility(ESlateVisibility::Collapsed);
		m_MainHUD->UI_OnOff(false);
	}
}

void APlayerCharacter::Quest()
{
	if (m_MainHUD->GetQuestWidget()->GetVisibility() == ESlateVisibility::Collapsed)
	{
		m_MainHUD->GetQuestWidget()->SetVisibility(ESlateVisibility::Visible);
		m_MainHUD->UI_OnOff(true);
	}
	else
	{
		m_MainHUD->GetQuestWidget()->SetVisibility(ESlateVisibility::Collapsed);
		m_MainHUD->UI_OnOff(false);
	}
}

void APlayerCharacter::OneKey()
{
	ItemUse(1);
}

void APlayerCharacter::TwoKey()
{
	ItemUse(2);
}

void APlayerCharacter::ThreeKey()
{
	ItemUse(3);
}

void APlayerCharacter::fourKey()
{
	ItemUse(4);
}

void APlayerCharacter::fiveKey()
{
	ItemUse(5);
}

void APlayerCharacter::sixKey()
{
	ItemUse(6);
}

void APlayerCharacter::ItemUse(int32 _UseNum)
{
	int32 UseitemNum = _UseNum - 1;
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), UseitemNum));
	if (m_UseInven[UseitemNum])
	{
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("dasdd")));
		if (UseitemNum != m_UseInvenNum)
		{
		
			m_EatItemOn = false;
			if (m_Weapon)
			{
				m_Weapon->Destroy();
				m_Weapon = nullptr;
				m_AnimInstance->SetWeapon(nullptr);
				m_AnimInstance->SetWeaponOnOff(false);
				m_WeaponOn = false;
			}
			if (m_UseInvenNum >= 0 && m_UseInven[m_UseInvenNum])
			{
				m_MainHUD->GetUseInventory()->UseOn(m_UseInvenNum, false);
				if (m_UseInven[m_UseInvenNum]->GetItemType() == EItemType::BuildItem)
				{	
					if(BuildManager->GetBuilding())
						ToggleBuildMode();
				}
			}
			
			m_UseInven[UseitemNum]->Use();
			m_UseInvenNum = UseitemNum;
			m_MainHUD->GetUseInventory()->UseOn(m_UseInvenNum, true);
		}
	}

}

void APlayerCharacter::Test() {
	
	/*
	if (!m_Weapon)
	{

		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABow* Bow = GetWorld()->SpawnActor<ABow>(BowAsset,
			FVector::ZeroVector, FRotator::ZeroRotator,
			param);

		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("Bow"));

		//Bow->SetMesh(TEXT("SkeletalMesh'/Game/DynamicCombatSystem/Meshes/SK_ElvenBow.SK_ElvenBow'"));
		
		
		m_Weapon = Cast<AWeapon>(Bow);
		m_AnimInstance->SetWeapon(m_Weapon);
	}
	else
	{

		m_Weapon->Destroy();
		m_Weapon = nullptr;
		m_AnimInstance->SetWeapon(nullptr);
		*/
	
}

void APlayerCharacter::WeapomSetSocket(const FString& Path)
{
	m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		FName(Path));

}


void APlayerCharacter::SetWeapon(AWeapon* _Weapon)
{
	 m_Weapon = _Weapon; m_AnimInstance->SetWeapon(_Weapon); 
}

void APlayerCharacter::SetCrosshair()
{
	
	UCrosshair* Cross = m_MainHUD->GetCrossHair();
	if (Cross)
	{
		FVector Location = m_ArrowSpawnLocation->GetComponentLocation() +(UKismetMathLibrary::GetForwardVector(Control_Rotation) *500.f);
	
		Cross->SetCrosshairPosition(Location);
		
	}
	
}

void APlayerCharacter::SetInvenItem(AUseItem* _Item, int32 _Num)
{
	if (m_UseInven[_Num])
	{
	
		m_MainHUD->GetUseInventory()->UseOn(_Num, false);

		if (_Num == m_UseInvenNum)
		{
			
			if (m_UseInven[m_UseInvenNum]->GetItemType() == EItemType::BuildItem) {
				if (BuildManager->GetBuilding())
					BuildManager->ToggleBuildMode();
			}
			else if (m_UseInven[m_UseInvenNum]->GetItemType() == EItemType::Weapon && m_Weapon)
			{
				m_Weapon->Destroy();
				m_Weapon = nullptr;
				m_AnimInstance->SetWeapon(nullptr);
				m_AnimInstance->SetWeaponOnOff(false);
				m_WeaponOn = false;
			}
			m_UseInvenNum = -1;
			m_EatItemOn = false;
		}
		
		
		
		m_UseInven[_Num]->Destroy();
		m_UseInven[_Num] = _Item;
	}
	else
	{
		m_UseInven[_Num] = _Item;
		m_UseInven[_Num]->SetOwner(this);
	}
	

}


FTransform APlayerCharacter::ArrowTransform()
{
	FVector L_ArrowSpawnLocation = m_ArrowSpawnLocation->GetComponentLocation();
	
	FVector L_CameraDirection = UKismetMathLibrary::GetDirectionUnitVector(m_Camera->GetComponentLocation(), (UKismetMathLibrary::GetForwardVector(Control_Rotation) * 500.f) + L_ArrowSpawnLocation);
	//FRotator L_ArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(L_CameraDirection); L_CameraDirection.Rotation();
	FRotator L_ArrowSpawnDirection = L_CameraDirection.Rotation();
	FVector L_LineTraceStart = m_Camera->GetComponentLocation() + (L_CameraDirection * UKismetMathLibrary::VSize(m_Arm->GetComponentLocation() - m_Camera->GetComponentLocation()));
	FVector	L_LineTraceEnd = (L_CameraDirection * 10000.0)+ m_Camera->GetComponentLocation();
	
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), L_ArrowSpawnDirection.Roll, L_ArrowSpawnDirection.Pitch, L_ArrowSpawnDirection.Yaw));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), L_CameraDirection.X, L_CameraDirection.Y, L_CameraDirection.Z));


	for (int32 i = 0; i <= 20; i++)
	{
		FVector L_CurrentTraceDirection = UKismetMathLibrary::GetDirectionUnitVector(L_LineTraceStart, (L_LineTraceEnd - FVector(0, 0, i * 5.f)));
		FHitResult	result;
		FCollisionQueryParams	params(NAME_None, false, this);
		bool Hit = GetWorld()->LineTraceSingleByChannel(result, (L_LineTraceStart - FVector(0, 0, i * 5.f)),
			(L_LineTraceEnd - FVector(0, 0, i * 5.f)), ECC_GameTraceChannel4, params);
		if (Hit)
		{
			FVector L_ImpactPoint = result.ImpactPoint;
			
			if (i == 0)
			{
				
				
				FVector L_CameraDirectionNormalize = L_CameraDirection;
				FVector L_CurrentTraceDirectionNormalize = L_CurrentTraceDirection;
				L_CameraDirectionNormalize.Normalize();
				L_CurrentTraceDirectionNormalize.Normalize();
				
				float  point = UKismetMathLibrary::VSize(m_Camera->GetComponentLocation() - L_ImpactPoint) /
					UKismetMathLibrary::DegCos(UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector
					(L_CameraDirectionNormalize, L_CurrentTraceDirectionNormalize)));

				
				//L_ArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(
					//UKismetMathLibrary::GetDirectionUnitVector(L_ArrowSpawnLocation, m_Camera->GetComponentLocation() + (point * L_CameraDirection)));
				L_ArrowSpawnDirection = UKismetMathLibrary::GetDirectionUnitVector(L_ArrowSpawnLocation, m_Camera->GetComponentLocation() + (point * L_CameraDirection)).Rotation();
				//FVector d = GetActorForwardVector() * 500.f;
			
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x %f,y %f,z %f"), d.X, d.Y, d.Z));
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Pitch %f,Yaw %f,Roll %f"), d.Pitch, d.Yaw, d.Roll));
				return FTransform(L_ArrowSpawnDirection, L_ArrowSpawnLocation);
			}
			else
			{
				if (UKismetMathLibrary::VSize(L_ArrowSpawnLocation - L_ImpactPoint) > 1000.f)
				{
					FVector L_CameraDirectionNormalize = L_CameraDirection;
					FVector L_CurrentTraceDirectionNormalize = L_CurrentTraceDirection;
					L_CameraDirectionNormalize.Normalize();
					L_CurrentTraceDirectionNormalize.Normalize();

					float  point = UKismetMathLibrary::VSize(m_Camera->GetComponentLocation() - L_ImpactPoint) /
						UKismetMathLibrary::DegCos(UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector
						(L_CameraDirectionNormalize, L_CurrentTraceDirectionNormalize)));

					//L_ArrowSpawnDirection = UKismetMathLibrary::MakeRotFromX(
						//UKismetMathLibrary::GetDirectionUnitVector(L_ArrowSpawnLocation, m_Camera->GetComponentLocation() + (point * L_CameraDirection)));
					L_ArrowSpawnDirection =
						UKismetMathLibrary::GetDirectionUnitVector(L_ArrowSpawnLocation, m_Camera->GetComponentLocation() + (point * L_CameraDirection)).Rotation();

					//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), L_ArrowSpawnDirection.Roll, L_ArrowSpawnDirection.Pitch, L_ArrowSpawnDirection.Yaw));

					//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f,%f,%f"), L_CameraDirection.X, L_CameraDirection.Y, L_CameraDirection.Z));
					return FTransform(L_ArrowSpawnDirection, L_ArrowSpawnLocation);
				}
					
			}

		}

	}

	

	return FTransform(L_ArrowSpawnDirection, L_ArrowSpawnLocation);
}

UBuildManagerComponent* APlayerCharacter::GetBuildManager() {
	return BuildManager;
}


UPlayerAnim* APlayerCharacter::GetPlayerAnim() {
	return m_AnimInstance;
}

void APlayerCharacter::NormalAttack() {
	

	

	FCollisionQueryParams	params(NAME_None, false, this);
	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	

	FVector Lo = GetActorLocation();

	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FHitResult>	HitResultArray;
	switch (m_Weapon->GetWeaponType())
	{
	case EWeaponType::Sword:
	{
		bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, Lo,
			Lo + GetActorForwardVector() * 140.f,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(50.f),
			params);
		/*
#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * 140.f;
		FVector Center = GetActorLocation() + TraceVec * 0.5f ;
		float HalfHeight = 140.f * 0.5f + 50.f;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = Sweep ? FColor::Red : FColor::Green;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			50.f,
			CapsuleRot,
			DrawColor,
			false,
			DebugLifeTime);

		
#endif
		*/
		if (Sweep)
		{
			for (auto& result : HitResultArray) {
				
				if (result.Actor.IsValid())
				{
					
					FDamageEvent DamageEvent;
					if (result.Actor->IsA(ABuildable::StaticClass()))
					{
						result.Actor->TakeDamage((float)m_Weapon->GetBuildAttack(), DamageEvent, GetController(), this);
					}
					else
					{
						result.Actor->TakeDamage((float)m_Weapon->GetAttack(), DamageEvent, GetController(), this);
						//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("ddd")));

						result.GetComponent()->OnComponentHit.Broadcast(nullptr, nullptr, nullptr, result.ImpactNormal, result);
					}
					//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), m_Weapon->GetAttack()));
					
					
					//result.Actor->NotifyHit(result.GetComponent(), result.GetActor(), nullptr, true, result.Location, result.Normal, result.ImpactNormal, result);
				}
			}
		}
		m_Engrgy -= 10.f;
	}
		break;
	case EWeaponType::Pick:
	{
		bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, Lo+FVector(0.f,0.f, 20.f),
			Lo + FVector(0.f, 0.f, 20.f) + GetActorForwardVector() * 96.f,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(32.f),
			params);
		/*
#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * 96.f;
		FVector Center = GetActorLocation() + TraceVec * 0.5f + FVector(0.f, 0.f, 20.f);
		float HalfHeight = 96.f * 0.5f + 32.f;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = Sweep ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			32.f,
			CapsuleRot,
			DrawColor,
			false,
			DebugLifeTime);

#endif
*/
		if (Sweep)
		{
			for (auto& result : HitResultArray) {
				if (result.Actor->IsA(AEnvironments::StaticClass()))
				{
					AEnvironments* Act = Cast<AEnvironments>(result.Actor);
					if (Act->GetEnvironmentsType() == EnvironmentsType::Stone)
					{
						Act->Attacked();
					}
				}
				else if (result.Actor.IsValid())
				{
					FDamageEvent DamageEvent;
					if (result.Actor->IsA(ABuildable::StaticClass()))
					{
						result.Actor->TakeDamage((float)m_Weapon->GetBuildAttack(), DamageEvent, GetController(), this);
					}
					else
					{
						result.Actor->TakeDamage((float)m_Weapon->GetAttack(), DamageEvent, GetController(), this);
						//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("ddd")));

						result.GetComponent()->OnComponentHit.Broadcast(nullptr, nullptr, nullptr, result.ImpactNormal, result);
					}
				}
			}
		}
		m_Engrgy -= 10.f;
	}
	break;
	case EWeaponType::Axe:
	{
		bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, Lo + FVector(0.f, 0.f, 25.f),
			Lo + FVector(0.f, 0.f, 25.f) + GetActorForwardVector() * 96.f,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(32.f),
			params);
		/*
#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * 96.f;
		FVector Center = GetActorLocation() + TraceVec * 0.5f + FVector(0.f, 0.f, 25.f);
		float HalfHeight = 96.f * 0.5f + 32.f;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = Sweep ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			32.f,
			CapsuleRot,
			DrawColor,
			false,
			DebugLifeTime);

#endif
*/
		if (Sweep)
		{
			for (auto& result : HitResultArray) {
				if (result.Actor->IsA(AEnvironments::StaticClass()))
				{
					AEnvironments* Act = Cast<AEnvironments>(result.Actor);
					if (Act->GetEnvironmentsType() == EnvironmentsType::Wood)
					{
						Act->Attacked();
					}
				}
				else if (result.Actor.IsValid())
				{
					FDamageEvent DamageEvent;
					if (result.Actor->IsA(ABuildable::StaticClass()))
					{
						result.Actor->TakeDamage((float)m_Weapon->GetBuildAttack(), DamageEvent, GetController(), this);
					}
					else
					{
						result.Actor->TakeDamage((float)m_Weapon->GetAttack(), DamageEvent, GetController(), this);
						//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("ddd")));

						result.GetComponent()->OnComponentHit.Broadcast(nullptr, nullptr, nullptr, result.ImpactNormal, result);
					}
				}
			}
		}
		m_Engrgy -= 10.f;
	}
		break;
	case EWeaponType::None:
		break;
	}
	
	
	

	
}


void APlayerCharacter::SetStateBar(UCharacterState* _StateBar){ 
	m_StateBar = _StateBar; }

void APlayerCharacter::PlayerCheckBeginOverlap()
{
	
	m_BuildableCheck = true;
	m_MainHUD->GetDurabilityBar()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void APlayerCharacter::PlayerCheckEndOverlap()
{
	m_BuildableCheck = false;
	
	m_MainHUD->GetDurabilityBar()->SetVisibility(ESlateVisibility::Collapsed);

	m_MainHUD->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
	if (m_NameDelegate.IsBound())
		m_NameDelegate.Unbind();
	m_CheckedBuild = nullptr;
	m_BuildUpgradeNum = -1;
}

void APlayerCharacter::Test22()
{
	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AMagicCircle>(AMagicCircle::StaticClass(),
		GetActorLocation(), FRotator::ZeroRotator, param);
}

void APlayerCharacter::SwordAttackEnd()
{
	m_SwordAttackEnable = true;
	m_SwordAttackIndex = 0;
}

void APlayerCharacter::SetGarmentMesh(const FUIItemTableInfo* _Info)
{
	

	if (_Info == nullptr)
	{
		m_GarmentOnOff = false;
		m_GarmentSkeletalMesh = nullptr;
		m_GarmentMesh->SetSkeletalMesh(m_GarmentSkeletalMesh);
		m_GarmentArmor = 0;
	}
	else
	{
		if (_Info->ItemType != EItemType::Equipment)
			return;
		m_GarmentOnOff = true;
		m_GarmentSkeletalMesh = _Info->SkeletalMesh;
		m_GarmentMesh->SetSkeletalMesh(m_GarmentSkeletalMesh);
		for (int i = 0; i < _Info->OptionArray.Num(); i++)
		{
			if (_Info->OptionArray[i].OptionType == EItemOption::Armor)
				m_GarmentArmor = _Info->OptionArray[i].Option;
		}
		
	}
	
}

void APlayerCharacter::Upgrade()
{
	if (m_BuildUpgradeNum == -1)
		return;
	UTileView* Inven = m_MainHUD->GetInventory()->GetInventorySlot();
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Inven->GetItemAt(m_BuildUpgradeNum));

	
	Item->SetAMount(Item->GetAMount() - 1);

	m_CheckedBuild->SetMesh(m_BuildUpgradeInfo->DestructibleMesh, m_BuildUpgradeInfo->Mesh);
	for (int32 i = 0; i < m_BuildUpgradeInfo->OptionArray.Num(); i++) {
		if (m_BuildUpgradeInfo->OptionArray[i].OptionType == EItemOption::Durability)
		{
			m_CheckedBuild->SetMaxDurability(m_BuildUpgradeInfo->OptionArray[i].Option);
			m_CheckedBuild->SetDurability(m_BuildUpgradeInfo->OptionArray[i].Option);
			switch (m_BuildUpgradeInfo->OptionArray[i].Option)
			{
			case 1000: m_CheckedBuild->SetMaterialType(EnvironmentsType::Wood);
				break;
			case 2000: m_CheckedBuild->SetMaterialType(EnvironmentsType::Stone);
				break;
			case 3000: m_CheckedBuild->SetMaterialType(EnvironmentsType::Metal);
				break;
			}
		}
	}

	if (Item->GetAMount() <= 0)
	{

		Item->SetNameText(TEXT(""));
		Item->SetIconTexture(nullptr);
		Item->SetUsePossible(false);
		Item->SetMaxAMount(0);
		Item->SetAMount(0);
		Item->SetItemIndex(0);
		m_MainHUD->GetInventory()->SetUseSlot(Item->GetIndex(), true);
	}
	Inven->RegenerateAllEntries();

	m_CheckedBuild = nullptr;
	m_BuildUpgradeNum = -1;
	m_MainHUD->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
	if (m_NameDelegate.IsBound())
		m_NameDelegate.Unbind();
	
	
}
