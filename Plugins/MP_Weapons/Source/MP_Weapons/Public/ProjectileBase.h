// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "IPoolableActor.h" // interface that pool calls on actor ( to : activate & deactivate object )
#include "IPoolable.h" // interface that this calls on/to pool
#include "ProjectileInterface.h"

#include "ProjectileBase.generated.h"


/*
 * Created on: 29/11/2025 | Last Change 07-Fev-2026
 */
UCLASS()
class MP_WEAPONS_API AProjectileBase : public AActor, public IIPoolableActor, public IProjectileInterface
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

	UFUNCTION()
	void Function_ResetProjectile(const FTransform InLaunchTransform);

	UFUNCTION()
	virtual void Function_EnterSleepState(); // added 05-Fev-2026

	// this function calls the pool interface function
	//UFUNCTION() // do i need to write this? what i gain (and lost) by writing it?
	virtual void Function_ReturnToPool(); // added on 26-Jan-2026


public:

	UPROPERTY()
	UActorComponent* OwnerPoolPtr; // this must be set by the weapon, cause on my system the pool is a ActorComponent that is attached to the weapon Actor

	// IPoolableActor
	virtual void IFunction_SetOnHold_Implementation() override;

	// via IIPoolableActor
	virtual void IFunction_ActivateActor_Implementation() override;

	// via IIPoolableActor
	virtual void IFunction_DeactivateActor_Implementation() override;

	// via IIPoolableActor
	virtual void IFunction_ResetActorWithTransform_Implementation(FTransform InTransform) override;

	// via IIPoolableActor
	virtual void IFunction_SetPointerToPoolComponent_Implementation(UActorComponent* InActorComponentPtr) override;

	// Implementation of ProjectileInterface
	//...
	virtual void IFunction_LaunchProjectile_Implementation(const FTransform InLaunchTransform) override;
};
