// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponPDA.generated.h"

// Created on: 29/11/2025
/**
 * 
 */
UCLASS()
class MP_WEAPONS_API UWeaponPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	// Weapon Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Base Class ]")
	FName WeaponName;

	// Weapon Description
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Base Class ]")
	FText WeaponDescription;

	// Weapon Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Base Class ]")
	UTexture2D* WeaponIcon;
	
};
