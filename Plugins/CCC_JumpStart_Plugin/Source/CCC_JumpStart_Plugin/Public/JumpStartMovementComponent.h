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
	CMM_Climb UMETA(DisplayName = "Climb")
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

	// Runtime...
	UPROPERTY(BlueprintReadWrite)
	FVector2D InputMoveValue; // <- Need to bind from Character to this

	float CapsuleRadius;
	bool bIsClimbing;
	FVector CurrentClimbNormal;
	FHitResult CurrentClimbHit;

	UPROPERTY(EditAnywhere, Category = "[ JumpStart Propertys ]|Debug")
	bool bDrawDebug;

public:
	/*
	* Added on 17-Fev-2026
	*/
	UFUNCTION()
	virtual void Function_ReadOwnerMovementInput(FVector2D InVector2D);

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
	* Added on 18-Fev-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ JumpStart UFunctions]")
	void Function_SetCapsuleRadiusRefValue(float InValue);

};
