// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetTree.h" // For "WidgetTree" access

#include "WD_HUD.generated.h"
// Created on 11/12/2025
/**
 * This is the HUD Widget class for the CCC JumpStart Plugin.
 * Offers basic HUD functionalities that can be found in many games. Giving feedback to the player or tradicional HUD elements.
 * 
 * A) Calculate and update a progress bar percentage based on current and maximum values.
 * 
 * B [ Crosshair ]
 * B.1) Show/hide crosshair + color coded based on what hit;
 * B.2) Weapon trace hit position on screen conversion if obstructed by objects;
 */
UCLASS()
class CCC_JUMPSTART_PLUGIN_API UWD_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override; // called when the widget is constructed
protected:
	// Default Widget Components | Default Widget Components | Default Widget Components | Default Widget Components |
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* RootCanvasPtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* CrosshairImagePtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* CrosshairObstructionImagePtr; // Added on 01/01/2026

	// Crosshair Icons | Crosshair Icons | Crosshair Icons | Crosshair Icons | Crosshair Icons |
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Icons") // Added on 11/12/2025
	UTexture2D* CrosshairIconPtr_Friendly;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Icons") // Added on 11/12/2025
	UTexture2D* CrosshairIconPtr_Enemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Icons") // Added on 11/12/2025
	UTexture2D* CrosshairIconPtr_Neutral;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Icons") // Added on 01/01/2026
	UTexture2D* CrosshairIconPtr_Obstructed;


	// Crosshair Colors | Crosshair Colors | Crosshair Colors | Crosshair Colors | Crosshair Colors |
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Colors") // Added on 11/12/2025
	FColor CrosshairColor_Friendly;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Colors") // Added on 11/12/2025
	FColor CrosshairColor_Enemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Jumpstart Widget ]|Crosshair|Colors") // Added on 11/12/2025
	FColor CrosshairColor_Neutral;


public:
	UFUNCTION(BlueprintCallable, Category = "[ Jumpstart Widget ]|Functions") // Added on 11/12/2025
	void Function_UpdateProgressBar(float inCurrentValue, float inMaxValue, float& outPercent);

	UFUNCTION(BlueprintCallable, Category = "[ Jumpstart Widget ]|Functions") // Added on 11/12/2025
	void Function_UpdateCrosshairState(UTexture2D* InCrosshairPtr, bool InShowCrosshair, FColor InColorTint);

	UFUNCTION(BlueprintCallable, Category = "[ Jumpstart Widget ]|Functions") // Added on 01/01/2026
	void Function_UpdateCrosshairObstructionPosition(FVector InWorldPos, bool InShowObstruction);
	
};
