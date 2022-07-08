// Copyright Epic Games, Inc. All Rights Reserved.


#include "PPGameModeBase.h"
#include "Character/Player/PlayerCharacter.h"

APPGameModeBase::APPGameModeBase() {

	
	//static ConstructorHelpers::FClassFinder<APawn>	PlayerBp(TEXT("Blueprint'/Game/Character/Player/BPPlayerCharacter.BPPlayerCharacter_C'"));
	/*
	static ConstructorHelpers::FClassFinder<APawn>	PlayerBp(TEXT("Blueprint'/Game/Character/Player/Character.Character_C'"));
	
	if (PlayerBp.Succeeded())
		DefaultPawnClass = PlayerBp.Class;
	*/
	static ConstructorHelpers::FClassFinder<UMainHUD>	MainHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainHUD.UI_MainHUD_C'"));

	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;

	
}
void APPGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void APPGameModeBase::BeginPlay()
{
	
	//Controller->bShowMouseCursor = true;
	//Controller->mouse
	
	
	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(),
			m_MainHUDClass));
	
		if (m_MainHUD)
		{
			m_MainHUD->AddToViewport();
			
		}
	}
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APlayerCharacter* Instance = Cast<APlayerCharacter>(GEngine->GameSingleton);

	
	APlayerCharacter* Effect = GetWorld()->SpawnActor<APlayerCharacter>(Instance->GetClass(),
		FVector(-70839.4375f, -113054.953125f, -4844.120605f), FRotator(0.f,180.f,0.f), param);
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly	Mode;



	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;

}
/*
void APPGameModeBase::Tick(float DeltaTime)
{
}
*/