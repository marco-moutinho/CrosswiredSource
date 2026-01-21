// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"

#include "ProjectileBase.h"

#include "ProjectileWeapon.generated.h"

class UObjectPoolComponent;
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

	// Override base weapon functions...
	virtual void Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA) override;

	UFUNCTION(BlueprintCallable)
	virtual void FunctionInitializeProjectilePool();

	// override parent
	virtual void Function_ExecuteWeaponAction() override;

	// Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions |
	
	/* ------------------------------------------------------------------------------------------
	* Trace a line to see if we hit anything, from the weapon's muzzle forward up to TraceRange;
	* ( public ) ( virtual )
	* [ Added on 29/11/2025 ] [ Updated on 13/jan/2026 ]
	--------------------------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_ShootWeaponTrace();

	/* 
	* 
	* Added on 19-jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Method_SetProjectileDestinationPoint(FVector InTargetLocation);

protected:

	/*
	* Spawns the projectile actor having in account the traces results;
	* ( protected ) ( virtual )
	* [ Added on 01/12/2025 ]
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Method_SpawnProjectile();

	/*
	* This function preloads and caches the projectile class when using a TSoftClassPtr.
	* this functions it is only needed if i use TSoftClassPtr / a soft ref/ptr to the projectile class instead of a hard ref...
	* call when the projectile type changes (equip / ammo change), so firing;
	* since this is a thing that needs to be allways loaded/ready to shoot, even if it can be any subclass of it and the player can change it at any time;
	* Created on 16 - Jan - 2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_LoadProjectileClass(TSoftClassPtr<AProjectileBase> InSoftProjectileClassPtr, TSubclassOf<AProjectileBase>& OutProjectileClassRef);

	// Added on 14-Jan-2026
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_InitializeProjectileSubClassData();

	/*
	* Sets the ProjectileClassRef
	* Added on 16-Jan-2027
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_SetProjectileClass(TSubclassOf<AProjectileBase> InProjectileClass);

	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Projectile Subclass")
	virtual void Function_Shoot();

	// Projectile Weapon Properties | Projectile Weapon Properties | Projectile Weapon Properties | Projectile Weapon Properties |
protected:
	/// [ Note on TSubclassOf vs TSoftClassPtr ]
	/// > Use TSubclassOf<> when:
	///		The class is hard-referenced
	///		Unreal loads it at startup
	///		Always in memory
	/// What happens: The class is hard-referenced; Unreal loads it at startup; Always in memory;
	/// Pros: Simple; No loading code; Safe to spawn anytime;
	/// Cons: Increases load time; Increases memory usage; Bad for large content libraries
	/// 
	/// > Use TSoftClassPtr<> when:
	///		The class is optional or data-driven
	///		Designers select it in a DataAsset / PDA
	///		You want lazy loading
	///		Typical for weapons, spells, projectiles
	/// What happens: Only the asset path is stored; Class is NOT loaded; Loaded only when you ask for it;
	/// Pros: Best for modular content; Faster startup; Lower memory usage; Scales well;
	/// Cons: Requires LoadSynchronous(); Slightly more code;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	TSubclassOf<AProjectileBase> _ProjectileClassRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	TSoftClassPtr<AProjectileBase> ProjectileClassPtr;

	// Trace range distance ( protected )
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	float _TraceRange;

	UPROPERTY(BlueprintReadWrite, Category = "[ RTO ]|Projectile Subclass")
	float _WeaponFireRate;

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
	
	UPROPERTY(BlueprintReadWrite, Category = "[ Weapon Comp ]")
	TObjectPtr<UObjectPoolComponent> PoolComponentPtr;
};
