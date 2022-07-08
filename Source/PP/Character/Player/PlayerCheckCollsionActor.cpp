// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCheckCollsionActor.h"
#include "../../PPGameInstance.h"
#include "../../PPGameModeBase.h"
#include "../../Widget/InventoryTile.h"
#include "PlayerCharacter.h"
// Sets default values
APlayerCheckCollsionActor::APlayerCheckCollsionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_CheckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckCollision"));
	m_BottleNum = -1;
	
}

// Called when the game starts or when spawned
void APlayerCheckCollsionActor::BeginPlay()
{
	Super::BeginPlay();
	m_CheckCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCheckCollsionActor::PlayerCheckBeginOverlap);
	m_CheckCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCheckCollsionActor::PlayerCheckEndOverlap);

	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	FString d = TEXT("Bottle");
	Bottle = GameInst->FindUIItemInfo(d);
	d = TEXT("WaterBottle");
	WaterBottle = GameInst->FindUIItemInfo(d);
}

// Called every frame
void APlayerCheckCollsionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCheckCollsionActor::PlayerCheckBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ("StaticMeshActor" == OtherActor->GetClass()->GetName())
	{
		
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		m_BottleNum = GameMode->GetMainHUD()->GetInventory()->FindItem(Bottle);
		if (m_BottleNum != -1)
		{
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("GetWater"))));
			Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetDelegate<APlayerCheckCollsionActor>(this, &APlayerCheckCollsionActor::GetWater);
		}
	}
	else
	{
		m_PlayerClass->PlayerCheckBeginOverlap();
		
		
	}
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Overlap")));
}

void APlayerCheckCollsionActor::PlayerCheckEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_BottleNum != -1)
	{
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
		Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->DelegateUnBind();
		m_BottleNum = -1;
	}
	else
	{
		m_PlayerClass->PlayerCheckEndOverlap();
	}
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("OverlapEnd")));
}

void APlayerCheckCollsionActor::GetWater()
{
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	
	UTileView* Inven =GameMode->GetMainHUD()->GetInventory()->GetInventorySlot();
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Inven->GetItemAt(m_BottleNum));
	
	Item->SetNameText(WaterBottle->Name);
	Item->SetIconTexture(WaterBottle->IconTexture);
	Item->SetUsePossible(WaterBottle->UsePossible);
	Item->SetMaxAMount(WaterBottle->MaxAMount);
	Item->SetAMount(WaterBottle->MaxAMount);
	Item->SetItemIndex(WaterBottle->ItemIndex);
	Inven->RegenerateAllEntries();

	m_BottleNum = GameMode->GetMainHUD()->GetInventory()->FindItem(Bottle);
	if (m_BottleNum == -1)
	{
		
		GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
		Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->DelegateUnBind();
	}
}
