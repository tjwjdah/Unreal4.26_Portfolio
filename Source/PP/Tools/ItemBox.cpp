// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "ItemBoxAnim.h"
#include "../PPGameInstance.h"
#include "../PPGameModeBase.h"
#include "../Widget/InventoryTile.h"
#include "../Widget/HotKey.h"
#include "../Character/Player/PlayerCharacter.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Box);

	m_Mesh->SetupAttachment(m_Box);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Item/ItemBox.ItemBox'"));

	//if (MeshAsset.Succeeded())
		//m_Mesh->SetStaticMesh(MeshAsset.Object);

	m_Box->SetCollisionProfileName(TEXT("PlayerTrigger"));
	m_DestroyTime = 0.f;
	m_DestroyOn = false;
	m_PlayerOn = true;

	m_BoxOn = false;
}

void AItemBox::ItemBoxClose()
{
	if (m_ItemBoxAnim->GetAnimType() != EBoxAnimType::Close && m_ItemBoxAnim->GetAnimType() != EBoxAnimType::Closed)
		m_ItemBoxAnim->ChangeAnimType(EBoxAnimType::Close);

}

void AItemBox::ItemBoxOpen()
{
	if (m_ItemBoxAnim->GetAnimType() != EBoxAnimType::Open && m_ItemBoxAnim->GetAnimType() != EBoxAnimType::Opened)
		m_ItemBoxAnim->ChangeAnimType(EBoxAnimType::Open);
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	m_Box->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::ItemBeginOverlap);
	m_Box->OnComponentEndOverlap.AddDynamic(this, &AItemBox::ItemEndOverlap);
	m_ItemBoxAnim = Cast<UItemBoxAnim>(m_Mesh->GetAnimInstance());
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_DestroyTime += DeltaTime;
	if (m_DestroyTime > 20.f && !m_DestroyOn && m_PlayerOn==false)
	{
		Destroy();
		m_DestroyOn = true;
	}
}

void AItemBox::ItemBeginOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	m_PlayerOn = true;
	
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{	
		// 인벤토리에 아이템을 추가한다.
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			GameMode->GetMainHUD()->GetHotKey()->SetKeyText(FText::FromString(FString::Printf(TEXT("E"))), FText::FromString(FString::Printf(TEXT("Loot"))));
				
		}
	
	}
	Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetDelegate<AItemBox>(this,&AItemBox::LootItemAddCallback);
}

void AItemBox::ItemEndOverlap(UPrimitiveComponent* OverlapCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_PlayerOn = false;
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	if (GameInst)
	{

		// 인벤토리에 아이템을 추가한다.
		APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->GetMainHUD()->GetLoot()->CloseButtonClick();
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
			GameMode->GetMainHUD()->GetLoot()->SetVisibility(ESlateVisibility::Collapsed);
				
				
		}
	}
	Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->DelegateUnBind();
	
}


void AItemBox::LootItemAddCallback() {
	UPPGameInstance* GameInst = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameInst)
	{
		if (m_BoxOn == false)
		{
			m_BoxOn = true;
			for (int32 i = 0; i < m_ItemNames.Num(); i++)
			{
				const FUIItemTableInfo* ItemInfo = GameInst->FindUIItemInfo(m_ItemNames[i].ItemName);

				if (ItemInfo)
				{
					// 인벤토리에 아이템을 추가한다.

					if (GameMode)
					{
						GameMode->GetMainHUD()->GetLoot()->AddItem(ItemInfo, m_ItemNames[i].aMount);
						GameMode->GetMainHUD()->GetLoot()->SetItemBox(this);
						m_ItemNames.RemoveAt(i);
						i--;
					}
				}
			}
			GameMode->GetMainHUD()->GetHotKey()->SetVisibility(ESlateVisibility::Collapsed);
			GameMode->GetMainHUD()->GetLoot()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemBoxOpen();
			APlayerController* pController = GetWorld()->GetFirstPlayerController();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->bShowMouseCursor = true;
		}
		else
		{
			m_BoxOn = false;
			GameMode->GetMainHUD()->GetLoot()->CloseButtonClick();
		}
	}

}
