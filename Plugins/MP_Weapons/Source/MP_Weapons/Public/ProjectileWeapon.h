// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"

#include "ProjectileBase.h"

#include "ProjectileWeapon.generated.h"

class UPoolComponent;
class UProjectileWeaponPDA;

/*
* This class suffers a big refactor to work symbiotically with Pool Component in order to gives better runtime performance
*/

/*
 * 
 * Created on : 29/11/2025 - Last change on 07-02-2026
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

	/*
	* This is called by WeaponUser before this is has finishSpawn
	*/
	virtual void Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA) override;

	UFUNCTION(BlueprintCallable)
	virtual void FunctionInitializeProjectilePool();

	// override parent
	virtual void Function_ExecuteWeaponAction() override;

	// Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions | Projectile Weapon Specific Functions |
	
	/*
	* [ Projectile Weapon Subclass Functions ]
	* setup stuff of projectile weapon subclass, such : PoolComponent, ProjectileClass etc
	* Created on 05-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_InitialSetup();

	/* [ Projectile Weapon Subclass Functions ]
	* ------------------------------------------------------------------------------------------
	* Trace a line to see if we hit anything, from the weapon's muzzle forward up to TraceRange;
	* ( public ) ( virtual )
	* [ Added on 29/11/2025 ] [ Updated on 13/jan/2026 ]
	--------------------------------------------------------------------------------------------*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual void Function_ShootWeaponTrace();

	/* 
	* [ Projectile Weapon Subclass Functions ]
	* Added on 19-jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual void Method_SetProjectileDestinationPoint(FVector InTargetLocation);

protected:
	/*
	* [ Projectile Weapon Subclass Functions ]
	* Called internally by "this->Function_SetProjectileSpawnTransform()"
	* Last change on 04-Fev-2026
	*/
	UFUNCTION()
	virtual FTransform Function_SetProjectileSpawnTransform();

	/*
	* [ Projectile Weapon Subclass Functions ]
	* This function preloads and caches the projectile class when using a TSoftClassPtr.
	* this functions it is only needed if i use TSoftClassPtr / a soft ref/ptr to the projectile class instead of a hard ref...
	* call when the projectile type changes (equip / ammo change), so firing;
	* since this is a thing that needs to be allways loaded/ready to shoot, even if it can be any subclass of it and the player can change it at any time;
	* Created on 16 - Jan - 2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual TSubclassOf<AProjectileBase> Function_LoadProjectileClass(TSoftClassPtr<AProjectileBase> InSoftProjectileClassPtr);

	/*
	* [ Projectile Weapon Subclass Functions ]
	* Added on 14-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual void Function_InitializeProjectileSubClassData();

	/*
	* [ Projectile Weapon Subclass Functions ]
	* Sets the ProjectileClassRef
	* Added on 16-Jan-2027
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual void Function_SetProjectileClass(TSubclassOf<AProjectileBase> InProjectileClass);

	/*
	* [ Projectile Weapon Subclass Functions ]
	* Full execution logic flow
	* 1 - Function_SetProjectileSpawnTransform() to set projectile "spawn" transform;
	* 2 - Access to the ObjectPoolComponent to call Function_RetrieveFromPoolWithTransform();
	* Added on 21-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Projectile Weapon Subclass Functions ]")
	virtual void Function_Shoot();

	// Projectile Weapon Properties
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

	// Set this to false if want to "override" the default cpp code, if yoy want to customize or change this class logic behavior via Blueprints or even write your own cpp code on top of it
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ Projectile Subclass Properties ]|Setup")
	bool bUseDefaultsCalls; /// WIP - I still have to implement this feature

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	TSubclassOf<AProjectileBase> _ProjectileSubClassRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	TSoftClassPtr<AProjectileBase> ProjectileSoftClassPtr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	int _ProjectileAmmount = 3; // check if changing parameter of the Actor on editor changes or this default value overrides it

	// Trace range distance ( protected )
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]")
	float _TraceRange;

	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	float _WeaponFireRate;

	// Trace hit result ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	FHitResult _TraceHitResult;

	// Trace origin point ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	FVector _TraceStartPoint;

	// Trace direction vector ( protected )
	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	FVector _TraceDirection;

	/*
	* Projectile Direction Vector
	* ( protected )
	* Added on 01/12/2025
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	FVector _ProjectileDirection;

	// Placeholder for trace calculations ( protected )
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|Placeholders")
	FVector Ph_StartPointOffset;

public:
	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	FVector _ProjectileDestinationPoint;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ Projectile Subclass Properties ]|Components")
	TObjectPtr<UPoolComponent> PoolComponentPtr;

	/*
	* [ Projectile Weapon Subclass Functions ]
	* Added on 06-Fev-2026
	*/
	UPROPERTY(BlueprintReadWrite, Category = "[ Projectile Subclass Properties ]|RTO")
	const UProjectileWeaponPDA* ProjectileWeaponDataPtr;
	// const makes so i cant change anything that the pointer is pointing to, such as values of hit, I can change the pointer it self, to ref other asset, I can call Functions stil
	
	private:
	float TimeToFire;
};
