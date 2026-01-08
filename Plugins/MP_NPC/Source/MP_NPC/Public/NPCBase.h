// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

/// <summary>
/// Created on 06/01/2026
/// </summary>
UCLASS()
class MP_NPC_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponentPtr;

	// added on 06/01/2026
	UFUNCTION(BlueprintCallable, Category = "[ NPC Functions ]")
	virtual void Function_InitializeNPC();
};
