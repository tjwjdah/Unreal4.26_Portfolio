// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"

AStartGameMode::AStartGameMode() {
	static ConstructorHelpers::FClassFinder<UUIStart>	StartHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainMenu.UI_MainMenu_C'"));

	if (StartHUDClass.Succeeded())
		m_StartHUDClass = StartHUDClass.Class;

}

void AStartGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_StartHUDClass))
	{
		m_StartHUD = Cast<UUIStart>(CreateWidget(GetWorld(),
			m_StartHUDClass));

		if (m_StartHUD)
			m_StartHUD->AddToViewport();
	}

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly	Mode;
	


	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;
}
