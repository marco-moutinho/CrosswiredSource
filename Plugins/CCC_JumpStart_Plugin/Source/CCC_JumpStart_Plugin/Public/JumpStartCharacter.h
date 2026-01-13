// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "JumpStartCharacter.generated.h"

// Forward declarations (faster compile, no full include here)
class USpringArmComponent;
class UCameraComponent;

// POV Enum
UENUM(BlueprintType) // Created on 13-jan-2026
enum class E_POV : uint8
{
	First UMETA(DisplayName = "First Person"),
	Third UMETA(DisplayName = "Third Person")
};

UCLASS()
class CCC_JUMPSTART_PLUGIN_API AJumpStartCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Jumpstart settings:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Settings")
	bool _AutoInit = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|ThirdPerson")
	float _ThirdPersonSpringArmLenght = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|ThirdPerson")
	bool _CanStrafeInThirdPerson = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Settings")
	E_POV _InitialPOV = E_POV::First;

	// Character Components...

	// USpringArm component created to jump start the character process;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " [ JumpStart Components ]", meta = (DisplayName = "JSSpringArm"))
	USpringArmComponent* M_SpringArm_Ptr;

	// UCamera component created to jump start the character process;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "[ JumpStart Components ]", meta = (DisplayName = "JSCamera"))
	UCameraComponent* M_Camera_Ptr;

	// Game feel / Tunning values:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Trace", meta = (DisplayName = "Range"))
	float _POVTraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Trace", meta = (DisplayName = "Radius"))
	float _POVSphereTraceRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Trace", meta = (DisplayName = "Profile"))
	FCollisionProfileName _POVTraceProfile;

	// POV trace hit result
	UPROPERTY(BlueprintReadWrite, Category = "[ JumpStart Run Time values ]")
	FHitResult M_POVTracehitResult;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "[ JumpStart Parameters ]|Helpers")
	bool M_ShowDebugDraws; bool M_ShowDebugMessage; bool M_DebugMode;


public:
	// Sets default values for this character's properties | this is built in method aka its automatic created by Unreal Engine
	AJumpStartCharacter();

protected:

	// Call on Construction
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "This sets the settings of: [ Character itself, CharacterMovementComponent, SpringArm, Camera ], so it behaves like a classic third person free-look character."))
	virtual void Function_SetThirdPersonControlSettings();

	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "This sets the settings of: [ Character itself, CharacterMovementComponent, SpringArm, Camera ], so it behaves like a classic first person character."))
	virtual void Function_SetFirstPersonControlSettings();

	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "Initializes the JumpStart Character by creating and setting up its components according to the specified parameters."))
	virtual void Function_InitializeJumpStartCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- --- --- --- --- --- PUBLIC JUMPSTART FUNCTIONS --- --- --- --- --- ---

	// Simple Move | This method is supposed to be called from a Player Controller
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "A single node that handles the basic movement input. Just to simplify basic staple behavior"))
	virtual void Function_MoveSimple(FVector2D In_Direction);

	// Simple Look | This method is supposed to be called from a Player Controller
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "A single node that handles a simple look behavior. \n(Rotates the SpringArm component of this character based on the player input received)"))
	virtual void Function_LookSimple(FVector2D In_Direction, float In_Sensitivity, bool In_IsInverted);

	// Trace from center of camera
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart Character ]", meta = (ToolTip = "Traces from center of screen/camera"))
	virtual void Function_TraceFromPOV();
	/// TO DO ?? : Make another method equal to the previus but that acepts parameters instead of use character parameters;

};
