// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UCrosshair::NativeConstruct()
{
	Super::NativeConstruct();
	m_CrosshairImage = Cast<UImage>(GetWidgetFromName(TEXT("CrosshairImage")));

	PanelSlot = Cast<UCanvasPanelSlot>(m_CrosshairImage->Slot);
}
void UCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCrosshair::SetCrosshairPosition(FVector _WorldLocation)
{
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(Controller, _WorldLocation, ScreenPosition, false);
	if (PanelSlot)
		PanelSlot->SetPosition(FVector2D(ScreenPosition.X,(ScreenPosition.Y)));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f %f"), ScreenPosition.X, 540.f - ScreenPosition.Y / 2));
}


