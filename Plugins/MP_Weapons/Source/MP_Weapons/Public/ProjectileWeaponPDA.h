// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponPDA.h"

#include "ProjectileBase.h"

#include "ProjectileWeaponPDA.generated.h"

/** Created on: [ 29/11/2025 ]
 * 
 */

UCLASS()
class MP_WEAPONS_API UProjectileWeaponPDA : public UWeaponPDA
{
	GENERATED_BODY()
	
public:

	// Range of the weapon's trace
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Weapon ]")
	float WeaponTraceRange;

	// Accuracy of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Weapon ]")
	float WeaponAccuracy;

	// Fire Rate of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Weapon ]")
	float WeaponFireRate;

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Projectile Weapon ]")
	TSoftClassPtr<AProjectileBase> ProjectileClassPtr;
};
