// Fill out your copyright notice in the Description page of Project Settings.


#include "Environments.h"

AEnvironments::AEnvironments()
{
	PrimaryActorTick.bCanEverTick = false;
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Box);
	static ConstructorHelpers::FClassFinder<AItemBox>	ItemBox(TEXT("Blueprint'/Game/Item/DropItem/BP_Box.BP_Box_C'"));
	if (ItemBox.Succeeded())
		m_DropItem = ItemBox.Class;
	m_Mesh->SetupAttachment(m_Box);
	m_Hp = 3;
	
}

void AEnvironments::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnvironments::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnvironments::Attacked() {
	m_Hp--;
	if (m_Hp == 0)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AItemBox* ItemBox = GetWorld()->SpawnActor<AItemBox>(m_DropItem->GetDefaultObject()->GetClass(),
			m_Mesh->GetComponentLocation() + FVector(0.f, 0.f, 50.f),
			m_Mesh->GetComponentRotation(), param);
		//아이템 박스 생성후 스폰
		for (int32 i = 0; i < m_DropItems.Num(); i++)
		{
			int32 aMount = FMath::RandRange(m_DropItems[i].MinaMount, m_DropItems[i].MaxaMount);
			if (aMount <=0)
				continue;
			FBoxItems Item;
			Item.aMount = aMount;
			Item.ItemName = m_DropItems[i].ItemName;
			ItemBox->AddItem(Item);
		}
		//생성한 아이템 박스에 드랍할 아이템 넣어줌
		Destroy();
	}
	
}


