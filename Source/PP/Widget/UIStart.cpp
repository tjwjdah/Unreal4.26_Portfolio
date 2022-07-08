// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStart.h"
#include "Components/Button.h"
#include "../PPGameModeBase.h"
#include "../Character/Player/PlayerCharacter.h"
#include "CharacterState.h"
#include "UseGarmentInventoryTile.h"
#include "UseInventoryTile.h"

void UUIStart::NativeConstruct()
{
	Super::NativeConstruct();

	m_BtnStart = Cast<UUIButton>(GetWidgetFromName(TEXT("BtnStart")));
	m_BtnEnd = Cast<UUIButton>(GetWidgetFromName(TEXT("BtnEnd")));
	
	BindingAnimations();

	m_BtnStart->GetButton()->OnClicked.AddDynamic(this, &UUIStart::StartBtn);
	m_BtnEnd->GetButton()->OnClicked.AddDynamic(this, &UUIStart::EndBtn);
	m_CameraMoveTime = 0.f;
	m_CameraMove = false;
	m_Test = 0;
}

void UUIStart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%d"), m_Test));

	if (m_CameraMove)
	{
		m_CameraMoveTime += InDeltaTime;
		if (m_CameraMoveTime>1.2f)
		{
			m_CameraMoveTime = 0.f;
			m_CameraMove = false;
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
			for (int32 i = 0; i < Actors.Num(); i++)
			{
				FString d = Actors[i]->GetName();

				if (Actors[i]->GetName() == TEXT("BPPlayerCharacter_C_0"))
				{
				
					APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
					GetWorld()->GetFirstPlayerController()->GetPawn()->Destroy();
					//GameMode->DefaultPawnClass = Cast<APawn>(Actors[i])->GetClass();
					GetWorld()->GetFirstPlayerController()->Possess(Cast<APawn>(Actors[i]));
					Cast<APlayerCharacter>(Cast<APawn>(Actors[i]))->PlayerStart();
					APlayerController* Controller = GetWorld()->GetFirstPlayerController();

					SetVisibility(ESlateVisibility::Collapsed);
					
					GameMode->GetMainHUD()->GetUseInventory()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					GameMode->GetMainHUD()->GetCharacterState()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					GameMode->GetMainHUD()->GetGarmentInventory()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					FInputModeGameAndUI	Mode;

					Controller->SetInputMode(FInputModeGameOnly());
					Controller->bShowMouseCursor = false;
					SetPlayer();

					m_Test = 1;
					break;

				}


			}
		}
	}
}

void UUIStart::BindingAnimations() {
	UProperty* Prop = GetClass()->PropertyLink;

	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* Anim = Cast<UWidgetAnimation>(Obj);

				if (Anim != nullptr)
				{
					if (Anim->MovieScene->GetFName() == "ShowMenu")
					{
						m_StartAnimation = Anim;
						LOG(TEXT("StartAnimation"));
						//Widget Animation Ã£±â
					}
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

void UUIStart::StartBtn()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	for (int32 i = 0; i < Actors.Num(); i++)
	{
		FString d = Actors[i]->GetName();

		if (Actors[i]->GetName() == TEXT("StartCamera_0") || Actors[i]->GetName() == TEXT("StartCamera"))
		{
			m_CameraMove = true;

			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Actors[i], 1.f);
			PlayAnimation(m_StartAnimation);
			
			break;
		}
	}
}


void UUIStart::EndBtn()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit, true);
}

void UUIStart::SetPlayer() {
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	UUseInventoryTile* UseInven = GameMode->GetMainHUD()->GetUseInventory();
	UseInven->SetPlayer(Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
	UUseGarmentInventoryTile* UseGarmentInven = GameMode->GetMainHUD()->GetGarmentInventory();
	UseGarmentInven->SetPlayer(Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
}