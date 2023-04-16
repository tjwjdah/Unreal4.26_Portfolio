// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "DestructibleComponent.h"
#include "Components/SplineComponent.h"
#include "Engine.h"
#include "Engine/AssetManager.h"
#include "EngineGlobals.h"
#include "NiagaraComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Animation/BlendSpace.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameInfo.generated.h"
/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(UEKR2, Log, All);
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(UEKR2, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGSTRING(Str)		UE_LOG(UEKR2, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

#define GET( type ,param) type Get##param () {return m_##param;}
#define SET( type ,param) void Set##param (type _in) {m_##param = _in;} 

#define GET_SET(type, param) GET(type,param) \
							 SET(type,param)


void PrintViewport(float Time, const FColor& Color, const FString& Text);


UENUM(BlueprintType)
enum class EItemOverlap : uint8
{
	None,
	Overlap
};
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Equipment,
	BuildItem,
	Eat,
	Material
};
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword,
	Bow,
	Pick,
	Axe,
	None
};
UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Foundation,
	Wall,
	Ceiling,
	DoorFrame,
	WindowFrame,
	Ramp,
	Stairs,
	Fence,
	Door,
	Window,
	None
};
UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Helmet,
	Armor,
	Pants,
	Shoes,
	Gloves,
	None
};

UENUM(BlueprintType)
enum class EItemOption : uint8
{
	Attack,
	Armor,
	HPMax,
	HPRecovery,
	EnergyRecovery,
	HungerRecovery,
	ThirstRecovery,
	Critical,
	Durability,
	BuildAttack
};

USTRUCT(BlueprintType)
struct FMonsterAttackInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			Attackdiameter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackAngle;

};

USTRUCT(BlueprintType)
struct FItemOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemOption	OptionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	Option;
};


USTRUCT(BlueprintType)
struct FDropItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	MaxaMount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	MinaMount;
};

USTRUCT(BlueprintType)
struct FMakeMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	aMount;
};
USTRUCT(BlueprintType)
struct FUIItemTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemOverlap	Overlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType	ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDestructibleMesh* DestructibleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	MaxAMount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FItemOption>	OptionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EWeaponType	WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EEquipmentType	EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EBuildType	EBuildType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FMakeMaterial>	MakeMaterials;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	UsePossible;

};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Collection,
	Hunt,
	Craft,
	Max
};
USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EQuestType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	DestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	Count;
};
USTRUCT(BlueprintType)
struct FQuestCompensation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	CompensationMount;
};

USTRUCT(BlueprintType)
struct FQuestTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString	QuestDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FQuestInfo>	InfoArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FQuestCompensation>	CompensationArray;
};

struct FQuestDataInfo {
	EQuestType	Type;
	FString	DestName;
	int32	Count;
	int32	MaxCount;
	bool Complete;
};


struct FQuestData
{
	FString	Name;
	FString	QuestDesc;
	TArray<FQuestDataInfo>	CompleteArray;
	TArray<FQuestCompensation>	CompensationArray;
	bool	Complete;
	int32 Index;
};

USTRUCT(BlueprintType)
struct FVoiceEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Jumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Landing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Breathing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Cheer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Pain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*>	Laugh;

};

UENUM(BlueprintType)
enum class ECheckDistanceType : uint8
{
	Trace,
	Attack
};


UENUM(BlueprintType)
enum class EExpression : uint8
{
	Neutral,
	Talking,
	Looking_Around
};

UENUM(BlueprintType)
enum class ECardinal_Direction : uint8
{
	North,
	East,
	South,
	West
};
UENUM(BlueprintType)
enum class EnvironmentsType : uint8
{
	Wood,
	Stone,
	Metal
};


UENUM(BlueprintType)
enum class EPhase : uint8
{
	Phase0,
	Phase1,
	Phase2
};


UENUM(BlueprintType)
enum class EState : uint8
{
	Start,
	Ing,
	End
};