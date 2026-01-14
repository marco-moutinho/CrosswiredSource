// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponPDA.h"

#include "ProjectileBase.h"

#include "ProjectileWeaponPDA.generated.h"

/*
 * Created on: [ 29/11/2025 ]
 * Last Change : 14-Jan-2026
 */

UCLASS()
class MP_WEAPONS_API UProjectileWeaponPDA : public UWeaponPDA
{
	GENERATED_BODY()
	
public:

	// Range of the weapon's trace
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Sub Class ]")
	float WeaponTraceRange;

	// Accuracy of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Sub Class ]")
	float WeaponAccuracy;

	// Fire Rate of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Sub Class ]")
	float WeaponFireRate;

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Sub Class ]")
	TSoftClassPtr<AProjectileBase> ProjectileClassPtr;
};
