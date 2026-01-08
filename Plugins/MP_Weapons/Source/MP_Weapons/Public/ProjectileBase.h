// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "ProjectileBase.generated.h"

// Created on: 29/11/2025
/*
 * 
 */
UCLASS()
class MP_WEAPONS_API AProjectileBase : public AActor
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


};
