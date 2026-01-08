// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

#include "WeaponHUD.generated.h"

/**
 * Created on 19/12/2025
 */
UCLASS()
class MP_WEAPONS_API UWeaponHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	// UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs |
	UPROPERTY(EditAnywhere, Category = "[ Weapon HUD Properties ]", meta = (BindWidgetOptional))
	UImage* WeaponIconPtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UCanvasPanel* CanvasPanelPtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_CurrentAmmoPtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_MaxAmmoPtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_WeaponNamePtr;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* ProgressBar_AmmoPtr;

public:
	// UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs | UFUNCTIONs |
	
	// added on 19/12/2025
	UFUNCTION(BlueprintCallable, Category = "[ Weapon HUD Functions ]")
	virtual void Function_UpdateCurrentWeaponIcon();
	
	// added on 20/12/2025
	/* Useful to energy based weapons
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon HUD Functions ]")
	virtual void Function_UpdateAmmoProgressBar(float InPercent, float InCurrent, float InMax);

	// added on 20/12/2025
	/*
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon HUD Functions ]")
	virtual void Function_UpdateAmmoText(int InCurrent, int InMax);
};
