// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHUD.h"

void UWeaponHUD::Function_UpdateCurrentWeaponIcon()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, this->GetName() + "UWeaponHUD::Function_UpdateCurrentWeaponIcon()"); } // Debug Msg call
}

void UWeaponHUD::Function_UpdateAmmoProgressBar(float InPercent, float InCurrent, float InMax)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, this->GetName() + "UWeaponHUD::Function_UpdateAmmoProgressBar()"); } // Debug Msg call
}

void UWeaponHUD::Function_UpdateAmmoText(int InCurrent, int InMax)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, this->GetName() + "UWeaponHUD::Function_UpdateAmmoText()"); } // Debug Msg call
}
