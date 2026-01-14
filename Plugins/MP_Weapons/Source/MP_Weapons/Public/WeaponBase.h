// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UWeaponDefinitionPDA;
class UCameraComponent;

/**
 * This class is meant to be inherited to create sub-classes to create specific weapon types and even sub types.
 * Created on : 29/11/2025 | Last Modified on : 14/12/2025
 */
UCLASS(Abstract, Blueprintable)
class MP_WEAPONS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Added on 29/11/2025
	// Called on the construction of the weapon to setup its graphics
	virtual void Method_InitializeWeaponVisuals();

	// Added on 15/12/2025
	// Should be called by weapon user to set the camera component pointer ??? or weapon owner character???
	virtual void Function_SetCameraComponentPointer(UCameraComponent* InCameraComponentPtr);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Base Weapon Functions | Base Weapon Functions |  Base Weapon Functions |  Base Weapon Functions |  Base Weapon Functions |  Base Weapon Functions |  Base Weapon Functions | 
	// Added on 14/12/2025
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Base")
	virtual void Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA);

	// Added on 14-Jan-2026
	UFUNCTION(BlueprintCallable, Category = "[ Weapon Functions ]|Base")
	virtual void Function_ExecuteWeaponAction();

	// Weapon Base Properties | Weapon Base Properties | Weapon Base Properties | Weapon Base Properties | Weapon Base Properties | Weapon Base Properties | Weapon Base Properties |
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon Properties ]|Debug")
	bool bDebugMode;

	/* Weapon 3D Graphics
	* This will be probably be replaced later on with a Skeletal Mesh Component so can have animations
	* Added on 29/11/2025
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Weapon Properties ]|Graphics")
	TObjectPtr<UStaticMeshComponent> WeaponMeshComp;

	// Its used to preform a trace from the camera to see where the player is aiming at
	UPROPERTY(BlueprintReadWrite, Transient, Category = "[ External Pointers ]|RTO")
	TObjectPtr<UCameraComponent> PlayerCameraCompPtr;

};
