// Fill out your copyright notice in the Description page of Project Settings.

#include "WD_HUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UWD_HUD::NativeConstruct()
{
	if (!RootCanvasPtr) {
		SET_WARN_COLOR(COLOR_RED);
		UE_LOG(LogTemp, Warning, TEXT("UWD_HUD::NativeConstruct: RootCanvasPtr is null."));
		return;
	}
	else
	{
		ELogVerbosity::Type LogVerbosity = ELogVerbosity::Log;
		//set log color to green
		SET_WARN_COLOR(COLOR_GREEN);
		UE_LOG(LogTemp, Warning, TEXT("UWD_HUD::NativeConstruct: RootCanvasPtr is valid."));
	}
}

void UWD_HUD::Function_UpdateProgressBar(float inCurrentValue, float inMaxValue, float& outPercent)
{
	outPercent = (inMaxValue != 0) ? (inCurrentValue / inMaxValue) : 0.0f;
}

void UWD_HUD::Function_UpdateCrosshairState(UTexture2D* InCrosshairPtr, bool InShowCrosshair, FColor InColorTint)
{
}

void UWD_HUD::Function_UpdateCrosshairObstructionPosition(FVector InWorldPos, bool InShowObstruction)
{
    if (!IsValid(CrosshairObstructionImagePtr))
        return;

    CrosshairObstructionImagePtr->SetVisibility(InShowObstruction ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    if (!InShowObstruction)
        return;

    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CrosshairObstructionImagePtr->Slot))
    {
		if (APlayerController* PC = GetOwningPlayer()) /// WIP : Set this only on BeginPlay and cache the pointer or OnInitialized
        {
            FVector2D WidgetPosition;

            // THIS is the correct API for UMG
            const bool bProjected = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, InWorldPos, WidgetPosition, false   // don't clamp to viewport
                );

            if (!bProjected) return;

            CanvasSlot->SetAnchors(FAnchors(0.f, 0.f));
            CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            CanvasSlot->SetPosition(WidgetPosition);
        }
    }
}
