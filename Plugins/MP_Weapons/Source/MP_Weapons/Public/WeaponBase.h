// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UWeaponPDA;
class UWeaponDefinitionPDA;
class UCameraComponent;

/**
 * This class is meant to be inherited to create sub-classes to create specific weapon types and even sub types.
 * Created on : 29/11/2025 | Last Modified on : 06/Fev/2026
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

	/*
	* [ WeaponBase Functions ]
	* Called on the construction of the weapon to setup its graphics
	* Added on 29/11/2025
	*/
	virtual void Method_InitializeWeaponVisuals();

	/*
	* [ WeaponBase Functions ]
	* Should be called by weapon user to set the camera component pointer ??? or weapon owner character???
	* Added on 15/12/2025
	*/
	virtual void Function_SetCameraComponentPointer(UCameraComponent* InCameraComponentPtr);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* [ WeaponBase Functions ]
	*/
	UFUNCTION(BlueprintCallable, Category = "[ WeaponBase Functions ]|Base")
	virtual void Function_InitializeExternalPointers(APawn* InOwningPawn);

	/*
	* [ WeaponBase Functions ]
	* This function is called by WeaponUser on Function_SpawnWeaponFromDefinition(), cause the weapon actor is spawned via "SpawnActorDeferred",
	* so it calls Function_InitializeFromDefinition/this before complete spawning(in world)
	* Added on 14/12/2025
	*/
	UFUNCTION(BlueprintCallable, Category = "[ WeaponBase Functions ]|Base")
	virtual void Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA);

	/*
	* [ WeaponBase Functions ]
	* This Function is empty and it's meant to be overrit on subclasses
	* Added on 14-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ WeaponBase Functions ]|Base")
	virtual void Function_ExecuteWeaponAction();

	/*
	* [ Base Propertys ]
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Base Properties ]|Debug")
	bool bDebugMode;

	/*
	* [ Base Propertys ]
	* Weapon 3D Graphics
	* This will be probably be replaced later on with a Skeletal Mesh Component so can have animations
	* Added on 29/11/2025
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Base Properties ]|Components")
	TObjectPtr<UStaticMeshComponent> WeaponMeshComp;

	/*
	* [ Base Propertys ] | 
	* Its used to preform a trace from the camera to see where the player is aiming at
	*/
	UPROPERTY(BlueprintReadWrite, Transient, Category = "[ Base Propertys ]|RTO")
	TObjectPtr<UCameraComponent> PlayerCameraCompPtr;

	/*
	* [ Base Propertys ] | RTO
	*/
	UPROPERTY(BlueprintReadWrite, Category = "[ Base Propertys ]|RTO")
	AActor* OwningActorPtr;
	
	/* 
	* [ Base Propertys ] | RTO
	*/
	UPROPERTY(BlueprintReadWrite, Category = "[ Base Propertys ]|RTO")
	APawn* OwningPawnPtr;

	/*
	* [ Base Propertys ] | RTO
	* Added on 06-Fev-2026
	*/
	UPROPERTY(BlueprintReadWrite, Category = "[ Base Propertys ]|RTO")
	UWeaponPDA* WeaponData;
};
