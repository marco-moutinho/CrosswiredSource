// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "IPoolableActor.h" // interface that pool calls on actor ( to : activate & deactivate object )
#include "IPoolable.h" // interface that this calls on/to pool

#include "ProjectileBase.generated.h"

// Created on: 29/11/2025
/*
 * 
 */
UCLASS()
class MP_WEAPONS_API AProjectileBase : public AActor, public IIPoolableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* Added on 29/11/2025
	*/
	virtual void Method_InitializeMeshComponent();

	/*
	* Added on 29/11/2025
	*/
	virtual void Method_InitializeProjectileComponent();

	/*
	* This function is binded to 
	* Added on 21-Jan-2026
	*/
	UFUNCTION()
	virtual void Function_OnProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit); // should i add "override"? the GPT said so...

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// projectile mesh component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "[ Projectile Class ]|Components")
	UStaticMeshComponent* _ProjectileMeshComponent;

	// projectile movement component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "[ Projectile Class ]|Components")
	UProjectileMovementComponent* _ProjectileMovementComponent;

	// set this when reset projectile, just to not add input parameters to the interface
	UPROPERTY(BlueprintReadWrite)
	FTransform _ResetTransform;

	UFUNCTION()
	void Function_ResetProjectile();

	// this function calls the pool interface function
	//UFUNCTION() // do i need to write this? what i gain (and lost) by writing it?
	virtual void Function_ReturnToPool(); // added on 26-Jan-2026


public:
	// This needs to be set on the actor spawn
	UPROPERTY() // <- do i need to write this? what i gain (and lost) by writing it?
	TScriptInterface<IIPoolable> OwnerPoolInterfacePtr;

	UPROPERTY()
	UActorComponent* OwnerPoolPtr; // this must be set by the weapon, cause on my system the pool is a ActorComponent that is attached to the weapon Actor

	// Inherited via IIPoolableActor
	virtual void IFunction_ActivateActor_Implementation() override;

	virtual void IFunction_DeactivateActor_Implementation() override;


	// Inherited via IIPoolableActor
	virtual void IFunction_ResetActorWithTransform_Implementation(FTransform InTransform) override;

};
