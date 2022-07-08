// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "../../Equipment/Bow.h"
#include "../../Equipment/Weapon.h"
UPlayerAnim::UPlayerAnim()
{
    //Tool_Idle_Pose ¼¼ÆÃ
    static ConstructorHelpers::FObjectFinder<UAnimSequence>	IdlePose_Bow(TEXT("AnimSequence'/Game/DynamicCombatSystem/AnimRetargeting/AR_Idle.AR_Idle'"));
    if (IdlePose_Bow.Succeeded())
    {
        BowIdlePose = IdlePose_Bow.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimSequence>	IdlePose_Axe(TEXT("AnimSequence'/Game/EasyBuildingSystem/Mannequin/AnimationsRetarget/A_Mannequin_Axe_Idle.A_Mannequin_Axe_Idle'"));
    if (IdlePose_Bow.Succeeded())
    {
        AxeIdlePose = IdlePose_Axe.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimSequence>	IdlePose_Pick(TEXT("AnimSequence'/Game/EasyBuildingSystem/Mannequin/AnimationsRetarget/A_Mannequin_Pickaxe_Idle.A_Mannequin_Pickaxe_Idle'"));
    if (IdlePose_Bow.Succeeded())
    {
        PickIdlePose = IdlePose_Pick.Object;
    }
    static ConstructorHelpers::FObjectFinder<UBlendSpace>	Moving_Blendspace_Bow(TEXT("BlendSpace'/Game/DynamicCombatSystem/AnimRetargeting/BS_ArcheryAim.BS_ArcheryAim'"));
    if (Moving_Blendspace_Bow.Succeeded())
    {
        BowMovingBlendspace = Moving_Blendspace_Bow.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBlendSpaceBase>	AO_Bow(TEXT("AimOffsetBlendSpace'/Game/DynamicCombatSystem/AnimRetargeting/AO_Archery_Aim.AO_Archery_Aim'"));
    if (AO_Bow.Succeeded())
    {
        BowAO = AO_Bow.Object;
    }
    static ConstructorHelpers::FObjectFinder<UBlendSpaceBase>	AO_Base(TEXT("AimOffsetBlendSpace'/Game/CharacterCustomizer/Character_Customizer/Characters/CC_Human/Animation/Animations/Aim_Offset/Idle/AO_Idle.AO_Idle'"));
    if (AO_Base.Succeeded())
    {
        BaseAO = AO_Base.Object;
    }


    static ConstructorHelpers::FObjectFinder<UAnimSequence>	AO_SwordBase(TEXT("AnimSequence'/Game/CharacterCustomizer/Tools/Tool_1H_Weapon/1H_Idle.1H_Idle'"));
    if (AO_SwordBase.Succeeded())
    {
        SwordIdlePose = AO_SwordBase.Object;
    }
    static ConstructorHelpers::FObjectFinder<UBlendSpace>	Moving_Blendspace_Sword(TEXT("BlendSpace'/Game/CharacterCustomizer/Tools/Tool_1H_Weapon/1H_Movement.1H_Movement'"));
    if (Moving_Blendspace_Sword.Succeeded())
    {
        SwordMovingBlendspace = Moving_Blendspace_Sword.Object;
    }


    WeaponOnOff = false;
    BowAimAlpha = 0.f;

    m_Death = false;
}
/*
bool UPlayerAnim::GetToolAiming()
{
    Active_Tool->GetAiming();
    return false;
}
*/

void UPlayerAnim::AnimNotify_DisarmBow()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    ABow* Bow = Cast<ABow>(Player->GetWeapon());
    Bow->ArrowVisibility(false);
    Player->WeapomSetSocket("bow");


    
}

void UPlayerAnim::AnimNotify_DrawoBow()
{

    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    ABow* Bow = Cast<ABow>(Player->GetWeapon());
    Bow->ArrowVisibility(true);
    Player->WeapomSetSocket("bow_use");
}
void UPlayerAnim::AnimNotify_Attack()
{

    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    if (Player)
        Player->NormalAttack();
}
void UPlayerAnim::SetWeapon(AWeapon* _Active_Weapon){

 
    
    Active_Weapon = _Active_Weapon;
    if (_Active_Weapon)
    {
        WeaponType = Active_Weapon->GetWeaponType();
        switch (Active_Weapon->GetWeaponType())
        {
        case EWeaponType::Sword:
        {
            Tool_Idle_Pose = SwordIdlePose;
            Tool_Moving_Blendspace = SwordMovingBlendspace;
            Tool_AO = BaseAO;
            
        }
            break;
        case EWeaponType::Bow:
        {
            Tool_Idle_Pose = BowIdlePose;
            Tool_Moving_Blendspace = BowMovingBlendspace;
            Tool_AO = BowAO;
        }
            break;
        case EWeaponType::Pick:
        {
            //PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Pickkkkk")));
            Tool_Idle_Pose = PickIdlePose;
            Tool_Moving_Blendspace = nullptr;
            Tool_AO = BaseAO;
        }
            break;
        case EWeaponType::Axe:
        {
            Tool_Idle_Pose = AxeIdlePose;
            Tool_Moving_Blendspace = nullptr;
            Tool_AO = BaseAO;
        }
            break;
        }
    }
    else
    {
       
        Tool_Idle_Pose = nullptr;
        Tool_Moving_Blendspace = nullptr;
        Tool_AO = BaseAO;

    }
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
    m_SwordAttack = false;
    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    if (Player)
        Player->SwordAttackEnd();
}

void UPlayerAnim::AnimNotify_NormalAttack()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    if (Player)
        Player->NormalAttack();
}

void UPlayerAnim::AnimNotify_AttackCombo()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

    if (Player)
        Player->EnableSwordAttack(true);
}

void UPlayerAnim::AnimNotify_HitEnd()
{
    Hit_Reaction = false;
    Hit_Direction = 0.f;
}


