// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// to make this include possible it was needed to go to ("project name".Build.cs) file and add "EnhancedInput" to the "PublicDependencyModuleNames"
// OR in this case being a plugin on the "Plugin Name".Build.cs
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include  "EnhancedInputComponent.h"

#include "Blueprint/UserWidget.h"
#include "WD_HUD.h"

#include "JumpStartController.generated.h"

/**
 * 
 */
UCLASS()
class CCC_JUMPSTART_PLUGIN_API AJumpStartController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// --- --- --- --- --- --- protected variables --- --- --- --- --- ---
	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Quick Start ]")
	UInputMappingContext* M_IMC_Controls_Ptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "[ Quick Start ]")
	int32 M_InputPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Quick Start ]")
	bool bAutoInitialize = true; // default true | Added on 01/01/2026

	// The defaults for gamepad analog stick and mouse are usually different
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Quick Start ]|Tunning")
	float M_LookSensitivity;

	// The defaults for gamepad analog stick and mouse are usually different
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Quick Start ]|Tunning")
	bool M_VerticalLookInverted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ Quick Start ]|UI")
	TSubclassOf<UWD_HUD> M_MainHudClass; // Added on 01/01/2026

	UPROPERTY(BlueprintReadWrite, Category = "[ Quick Start ]|UI")
	UWD_HUD* M_MainHudPtr; // Added on 01/01/2026
	

	// --- --- --- --- --- --- protected methods --- --- --- --- --- ---
	UFUNCTION(BlueprintCallable, Category = "[ Quick Start ]")
	virtual void Method_InitializeEnhancedInputSubsystem();
	
};
