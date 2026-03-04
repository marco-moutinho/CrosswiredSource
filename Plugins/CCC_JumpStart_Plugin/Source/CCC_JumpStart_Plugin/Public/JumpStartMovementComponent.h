// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpStartMovementComponent.generated.h"

/*
* Created on 17-02-2026
*/
UENUM(BlueprintType)
enum class EJumpStartMovementModes : uint8 {
	CMM_None UMETA(DisplayName = "None"),
	CMM_Climb UMETA(DisplayName = "Climb"),
	CMM_Dash  UMETA(DisplayName = "Dash")
};

USTRUCT(BlueprintType)
struct FClimbAngleParams
{
	GENERATED_BODY()

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MinUpwardsClimbAngle;

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MaxUpwardsClimbAngle;

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MinDownwardsClimbAngle;

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MaxDownwardsClimbAngle;

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MinLaterallyClimbAngle;

	// Angle in Degrees
	UPROPERTY(EditDefaultsOnly)
	float MaxLaterallyClimbAngle;
};

/*
* created 22-Fev-2026
*/
USTRUCT(BlueprintType)
struct  FDashParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float DashDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	float DashDistance = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	bool bDashCanChangeDirection = false;

	// TO DO : not yet implemented
	UPROPERTY(EditDefaultsOnly)
	bool bDashIgnoresGravity = true;

	// TO DO : not yet implemented
	UPROPERTY(EditDefaultsOnly)
	bool bPreserveVelocityAfterDash = true;

	/*
	* if true the dash is active till be forced to stop;
	* if true the dash velocity is decided by the DashSpeed (FDashParams.DashSpeed):
	*/
	UPROPERTY(EditDefaultsOnly)
	bool bDashWhileHoldingInput = false;

	// use DashSpeed on _DashSpeed if bDashWhileHoldingInput is true and un altered
	UPROPERTY(EditDefaultsOnly)
	float DashSpeed = 900.0f;

	// to implement this feature
	UPROPERTY(EditDefaultsOnly)
	float DashCooldown = 0.0f;

	/// Add a bool to : on hit during dash -> stop dash??
};


/**
 * Created on 17-02-2026
 */
UCLASS()
class CCC_JUMPSTART_PLUGIN_API UJumpStartMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	// Constructor
	UJumpStartMovementComponent();

	/*
	* This function is called every tick for custom movement modes, it is inherited from "UCharacterMovementComponent"
	* Added on 17-Fev-2026
	*/
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void BeginPlay() override;

protected:
	// vertical speed along the wall
	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb") 
	float ClimbSpeed = 300;

	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	float ClimbTraceForWallVerticalOffset = 60;

	// Smoothing speed for input velocity
	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	float ClimbAcceleration = 100;

	// Forward distance to detect wall
	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	float ClimbTraceDistance = 60;

	// How far from the wall the character capsule should stay, this is considering the capsule radius, so write only the distance between the surface and the "surface"/"skin" capsule 
	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	float ClimbWallOffset = 1;

	// Reject floors, only vertical-ish surfaces
	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	float MinClimbSurfaceDegrees = 0;

	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Climb")
	FClimbAngleParams ClimbAngleParams;

	UPROPERTY(EditDefaultsOnly, Category = "[ JumpStart Propertys ]|Dash")
	FDashParams DashParams;

	// Runtime...
	//...  input
	UPROPERTY(BlueprintReadWrite)
	FVector2D InputMoveValue; // <- Need to bind from Character to this

	// basic movement
	float _WalkSpeed;
	float _SprintSpeed;

	// climb
	bool bCanClimb;
	float CapsuleRadius;
	bool bIsClimbing;
	FVector CurrentClimbNormal;
	FHitResult CurrentClimbHit;

	// Dash float
	bool bCanDash;
	bool bIsDoingDash;
	float DashElapsedTime;
	FVector DashDirection;
	// _DashSpeed exist so it can be diffrent / modified run time without have to touch the original on the structure, so it can be used like ability modifier and still have a ref to the original value
	float _DashSpeed;
	float _DashCalculatedSpeed;
	// this var exist so it can be modified rto without touch the cooldown on the structure, so it can be easly reset to its default
	float _DashCooldownRTO;
	float _DashCooldownElapsedTime;

	UPROPERTY(EditAnywhere, Category = "[ JumpStart Propertys ]|Debug")
	bool bDrawDebug;

public:
	/*
	* Added on 17-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_ReadOwnerMovementInput(FVector2D InVector2D);

	/*
	* 22-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_SetIfCanClimb(bool InBool);

	/*
	* Added on 17-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_StartClimb();

	/*
	* Added on 17-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_StopClimb();

protected:
	/*
	* Core climb physics, called inside PhysCustom()
	* Added on 17-Fev-2026
	*/
	void Function_PhysClimb(float DeltaTime, int32 Iterations);

	/*
	* Detect if a climbable wall is in front
	* Added on 17-Fev-2026
	*/
	bool Function_HasClimbableSurface(FHitResult& OutHit) const;

public:
	/*
	* Created on 22-Fev-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions ]|Dash")
	virtual void Function_SetDashParameters(FDashParams InParams);

	// Created on 22-Fev-2026
	/*
	* CanChange Dash Direction ?
	* If false uses the input vector parameter;
	* If true uses the last input read;
	*/
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions ]|Dash")
	void Function_StartDash(FVector InDirection);

protected:
	/*
	* Created on 22-Fev-2026
	*/
	void Function_StopDash();

	/*
	* Created on 22-Fev-2026
	*/
	virtual void Function_PhysDash(float InDeltaTime, int32 Iterations);

public:
	// created 03-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions ]|Dash")
	virtual void Function_ForceStopDash();

	// created on 04-Mar-2026
	/*
	* this changes the value of _DashSpeed not the FDashParams.DashSpeed
	*/
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions ]|Dash")
	virtual void Function_SetDashSpeed(float inValue);

	/*
	* Added on 18-Fev-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions ]|Geral")
	void Function_SetCapsuleRadiusRefValue(float InValue);

};

/// TO DO:
/// currently its possible to spam the dash, add a way of limit that??

/// KNOWN BUGS
/// Start Dash right after Start Jump seems to stop movement (mid air);