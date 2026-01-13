// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"

#include "ProjectileBase.h"

#include "ProjectileWeapon.generated.h"

/**
 * Created on : 29/11/2025
 */
UCLASS()
class MP_WEAPONS_API AProjectileWeapon : public AWeaponBase
{
	GENERATED_BODY()

	// Override Constructor | Override Constructor | Override Constructor | Override Constructor |
public:
	// Sets default values for this actor's properties
	AProjectileWeapon();

	// Override base weapon functions | Override base weapon functions | Override base weapon functions | Override base weapon functions |

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void Function_StartUse() override;
	virtual void Function_StopUse() override;
	virtual void Method_Using() override;

	// Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions |
	
	/* ------------------------------------------------------------------------------------------
	* Trace a line to see if we hit anything, from the weapon's muzzle forward up to TraceRange;
	* ( public ) ( virtual )
	* [ Added on 29/11/2025 ] [ Updated on 13/jan/2026 ]
	--------------------------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_ShootWeaponTrace();

	/* 
	* Set the projectile destination based on the trace hit result;
	* ( public ) ( virtual )
	* [ Added on 01/12/2025 ]
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Method_SetProjectileDestinationPoint();

protected:

	/*
	* Spawns the projectile actor having in account the traces results;
	* ( protected ) ( virtual )
	* [ Added on 01/12/2025 ]
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Method_SpawnProjectile();



	// Projectile Weapon Properties | Projectile Weapon Properties | Projectile Weapon Properties | Projectile Weapon Properties |
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	TSubclassOf<AProjectileBase> _ProjectileClass;

	// Trace range distance ( protected )
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	float _TraceRange;

	// Trace hit result ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	FHitResult _TraceHitResult;

	// Trace origin point ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	FVector _TraceStartPoint;

	// Trace direction vector ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	FVector _TraceDirection;

	/*
	* Projectile Direction Vector
	* ( protected )
	* Added on 01/12/2025
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	FVector _ProjectileDirection;

	// Placeholder for trace calculations ( protected )
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Placeholder ]")
	FVector Ph_StartPointOffset;

public:
	UPROPERTY(BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	FVector _ProjectileDestinationPoint;
	
};
