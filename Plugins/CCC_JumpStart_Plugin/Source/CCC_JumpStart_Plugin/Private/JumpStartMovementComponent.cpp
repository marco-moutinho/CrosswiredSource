// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpStartMovementComponent.h"

UJumpStartMovementComponent::UJumpStartMovementComponent()
{
	bIsClimbing = false;
	bCanDash = true;
}

void UJumpStartMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	// handle Climb mode
	if (CustomMovementMode == (uint8)EJumpStartMovementModes::CMM_Climb && bCanClimb) {
		Function_PhysClimb(DeltaTime, Iterations);
		return;
	}

	// Handle Dash mode
	if (CustomMovementMode == (uint8)EJumpStartMovementModes::CMM_Dash && bCanDash) {
		Function_Dash(DeltaTime);
	}

	// Otherwise fallback to normal custom physics
	Super::PhysCustom(DeltaTime, Iterations); // PhysCustom is called every physics tick. 
}

void UJumpStartMovementComponent::Function_ReadOwnerMovementInput(FVector2D InVector2D)
{
	InputMoveValue = InVector2D;
}

void UJumpStartMovementComponent::Function_SetIfCanClimb(bool InBool)
{
	bCanClimb = InBool;
}

void UJumpStartMovementComponent::Function_StartClimb()
{
	// check if is already climbing
	if (bIsClimbing) { return; }
	// if not then check if has any wall/surface to climb

	FHitResult LcHitresult;
	if (Function_HasClimbableSurface(LcHitresult)) {
		
		// wall is valid -> start climbing
		bIsClimbing = true;
		CurrentClimbHit = LcHitresult;
		CurrentClimbNormal = LcHitresult.ImpactNormal;

		// Set custom movement mode "Climb" to current move mode
		SetMovementMode(MOVE_Custom, (uint8)EJumpStartMovementModes::CMM_Climb);
	
		// Stop any existing momentum
		/// FUTURE : Idk maybe i want to preserve some momentum, mainly in slippery surfaces
		Velocity = FVector::ZeroVector;
		
		// Finish
		return;
	}
	else { return; }
}

void UJumpStartMovementComponent::Function_StopClimb()
{
	if (bIsClimbing == false) return;

	bIsClimbing = false;

	// Set movement mode back to falling
	SetMovementMode(MOVE_Falling);
}

void UJumpStartMovementComponent::Function_PhysClimb(float DeltaTime, int32 Iterations)
{
	//  Step 0 :Must have a valid Owner
	if (!PawnOwner) { Function_StopClimb(); return; }

	// Step 1 : Check if wall is still valid
	FHitResult LcHit;
	//FVector Start = PawnOwner->GetActorLocation();
	//FVector End = Start + (-CurrentClimbNormal * ClimbTraceDistance); // trace backward to stick to wall

	FVector LcStartOffset = (CapsuleRadius * PawnOwner->GetActorForwardVector()) + (PawnOwner->GetActorUpVector() * ClimbTraceForWallVerticalOffset);
	FVector LcStart = PawnOwner->GetActorLocation() + LcStartOffset;
	FVector LcEnd = LcStart - CurrentClimbNormal * ClimbTraceDistance; // trace backward to stick to wall

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PawnOwner);

	// perform trace
	bool bStillOnWall = GetWorld()->LineTraceSingleByChannel(LcHit, LcStart, LcEnd, ECC_Visibility, Params);

	// DEBUG DRAW
	if (bDrawDebug) { DrawDebugLine(GetWorld(), LcStart, LcEnd, FColor::Magenta, false, -1, 0, 1); }

	if (!bStillOnWall)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, "No longer has wall to climb"); } // this line is firing constatly , i think it has to do with snap math or something
		// Wall disappeared -> stop climbing
		Function_StopClimb();
		return;
	}

	//Step2 v3
	FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	FVector WallNormal = LcHit.ImpactNormal.GetSafeNormal();
	FVector WallPoint = LcHit.ImpactPoint;

	// Project onto wall plane along the wall normal
	FVector ToWall = ComponentLocation - WallPoint;
	float DistanceAlongNormal = FVector::DotProduct(ToWall, WallNormal);

	// Compute snapped location relative to component
	FVector SnappedLocation = ComponentLocation - WallNormal * (DistanceAlongNormal - ClimbWallOffset);

	// Keep Z the same if you want
	SnappedLocation.Z = ComponentLocation.Z;

	// Use SafeMoveUpdatedComponent correctly
	FHitResult SnapHit;
	FVector Delta = SnappedLocation - ComponentLocation; // this must be small, relative to component
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, SnapHit);

	// Step 3: Build movement vector
	/*FVector LcVerticalMovement = FVector::UpVector * ClimbSpeed * InputMoveValue.Y;
	FVector LcHorizontalMovement = FVector::CrossProduct(FVector::UpVector, CurrentClimbNormal).GetSafeNormal() * ClimbSpeed * InputMoveValue.X;
	FVector LcClimbDelta = LcHorizontalMovement + LcVerticalMovement;*/

	//Velocity = LcClimbDelta;
	//Velocity = FVector::Zero();

	// Step 4 : Apply movement
	//PawnOwner->AddActorWorldOffset(Velocity * DeltaTime, true); <- this seems to has worked

	FVector WallRight = FVector::CrossProduct( WallNormal, FVector::UpVector).GetSafeNormal();
	FVector WallUp = FVector::UpVector;

	FVector MoveDelta = WallRight * InputMoveValue.X * ClimbSpeed * DeltaTime + WallUp * InputMoveValue.Y * ClimbSpeed * DeltaTime;
	FHitResult MoveHit;
	SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentQuat(), true, MoveHit);

	/*
	* Vector Notes:
	* CrossProduct will always bec a perpendicular vector relative to given axis(vectors)
	*/
}

bool UJumpStartMovementComponent::Function_HasClimbableSurface(FHitResult& OutHit) const
{
	// Step 0 : Must have a valid character
	if (!PawnOwner) { return false; }

	// Step 1: Start and End of trace
	FVector LcStartOffset = (CapsuleRadius * PawnOwner->GetActorForwardVector()) + (PawnOwner->GetActorUpVector() * ClimbTraceForWallVerticalOffset);
	FVector LcStart = PawnOwner->GetActorLocation() + LcStartOffset; // WIP + some vertical offset?!!
	FVector LcForward = PawnOwner->GetActorForwardVector();
	FVector LcEnd = LcStart + (LcForward * ClimbTraceDistance);

	// Step 2: Collision parameters
	FCollisionQueryParams LcCollisionQueryParams;
	LcCollisionQueryParams.AddIgnoredActor(PawnOwner);

	// Step 3: Perform line trace
	bool bLcHit = GetWorld()->LineTraceSingleByChannel(OutHit, LcStart, LcEnd, ECC_Visibility, LcCollisionQueryParams);

	// DEBUG DRAWs
	if(bDrawDebug){
		if (bLcHit == true) {DrawDebugLine(GetWorld(), LcStart, OutHit.ImpactPoint, FColor::Green, false, 2, 0, 1); }
		else { DrawDebugLine(GetWorld(), LcStart,LcEnd, FColor::Red, false, -1, 0, 1); }
	}

	// Finish and return false is line trace did not hit anything
	if (bLcHit == false) { return false; }

	// Step 4 : Check Angle
	// calculate angle of normal of the surface to check if the angle is climbable
	FVector LcNormal = OutHit.ImpactNormal;
	float LcSurfaceAngleInDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LcNormal, FVector::UpVector)));
	if (LcSurfaceAngleInDegrees < MinClimbSurfaceDegrees) // W I P : ADD || LcSurfaceAngleInDegrees > MAXClimbSurfaceDegrees
	{
		return false;
	}
	
	return true;
}

void UJumpStartMovementComponent::Function_SetDashParameters(FDashParams InParams)
{
	DashParams = InParams;
}

void UJumpStartMovementComponent::Function_StartDash(FVector InDirection)
{
	DashDirection = InDirection;
	DashElapsedTime = 0;
	bIsDoingDash = true;

	SetMovementMode(MOVE_Custom, (uint8)EJumpStartMovementModes::CMM_Dash);
}

void UJumpStartMovementComponent::Function_StopDash()
{
	bIsDoingDash = false;

	// On Stop doing Dash check if is grounded to set next movement mode to walk or falling
	FFindFloorResult LcFindFloorResult;
	FindFloor(PawnOwner->GetActorLocation(), LcFindFloorResult, false);
	if (LcFindFloorResult.bBlockingHit) {
		SetMovementMode(MOVE_Walking);
	}
	else {
		SetMovementMode(MOVE_Falling);
	}
}

void UJumpStartMovementComponent::Function_Dash(float InDeltaTime)
{
	DashElapsedTime += InDeltaTime;
	if (DashElapsedTime >= DashParams.DashDuration) {
		Function_StopDash();
		return;
	}

	FVector LcDirection;

	// case can't change direction while perform a dash
	if (DashParams.bDashCanChangeDirection == true) {  LcDirection = GetLastInputVector(); }

	// case it can change direction while perform a dash
	else { LcDirection = DashDirection; }

	// cancel dash if LcDirection is ZERO
	if (LcDirection.GetSafeNormal() == FVector::Zero()) {
		this->Function_StopDash();
		return;
	}
	
	// Known "BUG" - If DashDirection is ZERO the character just stands still
	// it can be a feature actualy right?
	
	// create a delta vector
	FVector LcDelta;

	// case dash has manual duration
	if (DashParams.bDashWhileHoldingInput == true) {
		LcDelta = LcDirection.GetSafeNormal() * _DashSpeed;
	}

	// case dash has auto duration
	else if (DashParams.bDashWhileHoldingInput == false) {
		LcDelta = LcDirection.GetSafeNormal() * (DashParams.DashDistance / DashParams.DashDuration);
	}
	
	// Make it frame rate independent
	LcDelta *= InDeltaTime;

	FHitResult LcHitResult;

	// Apply movement
	SafeMoveUpdatedComponent(LcDelta, UpdatedComponent->GetComponentQuat(), true, LcHitResult);
}

void UJumpStartMovementComponent::Function_ForceStopDash()
{
	this->Function_StopDash();
}

void UJumpStartMovementComponent::Function_SetCapsuleRadiusRefValue(float InValue)
{
	CapsuleRadius = InValue;
}
