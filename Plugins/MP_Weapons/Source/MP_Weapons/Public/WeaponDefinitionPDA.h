// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "WeaponBase.h"
#include "WeaponPDA.h"
#include "ProjectileBase.h"

#include "WeaponDefinitionPDA.generated.h"

// Created on 13/12/2025
/**
 * 
 */
UCLASS()
class MP_WEAPONS_API UWeaponDefinitionPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Weapon Actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "[ Weapon Definition Properties ]|Base Class")
	TSoftClassPtr<AWeaponBase> WeaponSoftClassPtr;

	// Weapon Icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "[ Weapon Definition Properties ]|Base Class")
	TSoftObjectPtr<UTexture2D> WeaponIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "[ Weapon Definition Properties ]|Base Class")
	TSoftObjectPtr<UWeaponPDA> WeaponDataPDA;

};
