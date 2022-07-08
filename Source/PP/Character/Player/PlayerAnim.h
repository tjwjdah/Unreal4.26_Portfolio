// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "../../Customizer/CharacterCustomizer.h"


#include "PlayerAnim.generated.h"
/**
 * 
 */
UCLASS()
class PP_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCharacterCustomizer* CharacterCustomizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LegIK)
		float L_Foot_Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LegIK)
		float R_Foot_Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LegIK)
		FRotator L_Foot_Rot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = LegIK)
		FRotator R_Foot_Rot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		FVector Head_Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		FVector Foot_Transforms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		float Breast_Size_Alpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		FRotator Look_At_Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		int32 Voice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		FVoiceEffects Voices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		UAnimSequenceBase* Proportional_Additive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = CC)
		EExpression Current_Expression;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Lean)
		float Lean_Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Lean)
		float Lean_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Yaw_Last_Frame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Yaw_Delta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float RYO;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float RYO_Directional_Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float RYO_Directional_Offset_Last_Frame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Curve_Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		bool Curve_bool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Curve_Value_Last_Frame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Turn_Ratio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		float Spin_Value_Last_Frame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Rotation)
		ECardinal_Direction Cardinal_Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Slope)
		float Slope_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		UBlendSpace* Tool_Moving_Blendspace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		UAnimSequence* Tool_Idle_Pose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		UAnimSequence* Tool_Aiming_Pose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		UBlendSpaceBase* Tool_AO;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		bool Hit_Reaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		float Hit_Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		class AWeapon* Active_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		FVector FABRIK_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		bool Left_Hand_IK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		bool UpperBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		bool Aiming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		bool WeaponOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Tool)
		float Left_Arm_Blend_Out;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Falling)
		bool In_Air;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Falling)
		float Time_In_Air;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Falling)
		float Fall_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Falling)
		float Ground_Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Pivot)
		float Direction_Delta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Pivot)
		float Direction_Last_Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Aiming)
		float Yaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Aiming)
		float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Aiming)
		FRotator Control_Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Aiming)
		float		BowAimAlpha;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		bool Accelerating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		bool Accelerating_Last_Frame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		FVector Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Move)
		bool Sprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_Death;
	
	UAnimSequence* SwordIdlePose;
	UAnimSequence* BowIdlePose;
	UAnimSequence* AxeIdlePose;
	UAnimSequence* PickIdlePose;
	UBlendSpace* BowMovingBlendspace;
	UBlendSpace* SwordMovingBlendspace;

	
	UBlendSpaceBase* BowAO;
	
	UBlendSpaceBase* BaseAO;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_SwordAttack;

public:
	/*
	UFUNCTION(BlueprintPure)
		bool GetToolAiming();
		*/
	UFUNCTION()
		void AnimNotify_DisarmBow();
	UFUNCTION()
		void AnimNotify_DrawoBow();
	UFUNCTION()
		void AnimNotify_Attack();

public:
	void SetAiming(bool _Aiming) { Aiming = _Aiming; }
	void SetWeapon(class AWeapon* _Active_Weapon);
	void SetWeaponOnOff(bool _OnOff) { WeaponOnOff = _OnOff; }
	float GetBowAimAlpha() { return BowAimAlpha; }
	void SetRyo(float _Ryo) { RYO = _Ryo;  Curve_Value = 0.f; Curve_Value_Last_Frame = 0; Curve_bool = false; }
	void SetSwordAttackEnable(bool Attack)
	{
		m_SwordAttack = Attack;
	}
	bool IsSwordAttack()	const
	{
		return m_SwordAttack;
	}

	void SetDeath(bool _Death) { m_Death = _Death; }
	bool GetDeath() { return m_Death; }

	void SetHit(bool _HitReaction) { Hit_Reaction = _HitReaction;}
	bool GetHit() { return Hit_Reaction; }
	void SetHitDirection(float _SetHitDirection) { Hit_Direction = _SetHitDirection; }
public:
		// 노티파이가 호출할 함수를 만들때는 반드시 규칙을 지켜야 한다.
		// AnimNotify_ 를 한 뒤에 노티파이 이름으로 함수를 만들어준다.
		UFUNCTION()
			void AnimNotify_AttackEnd();

		UFUNCTION()
			void AnimNotify_NormalAttack();

		UFUNCTION()
			void AnimNotify_AttackCombo();

		UFUNCTION()
			void AnimNotify_HitEnd();

};


