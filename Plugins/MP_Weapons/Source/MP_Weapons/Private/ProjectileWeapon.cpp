// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

AProjectileWeapon::AProjectileWeapon()
{
	// Call parent constructor -  IP : Is this needed? Need to checkout
	//Super::AWeaponBase();

	// Enable Tick function
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickEnabled(false); // Enabled by default for testing, can be disabled later on

	//Set tick group
	PrimaryActorTick.TickGroup = TG_LastDemotable;
}

void AProjectileWeapon::BeginPlay()
{
	// Call parent BeginPlay
	Super::BeginPlay();
}

void AProjectileWeapon::Tick(float DeltaTime)
{
	// Call parent Tick
	Super::Tick(DeltaTime);

	//// Debug message to indicate Tick call
	//if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, GetName() + " | AProjectileWeapon::Tick()"); }

	// Call Shoot Trace method
	Function_ShootWeaponTrace();
}

void AProjectileWeapon::Function_StartUse()
{
	// Debug message to indicate method call...
	if (GEngine && bDebugMode) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Silver,
			"[" + this->GetName() + "]" + " AProjectileWeapon::Method_StartUse()" +
			" | Called from : " + GetOwner()->GetName());
	}
}

void AProjectileWeapon::Function_StopUse()
{
}

void AProjectileWeapon::Method_Using()
{
}

// WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP |
void AProjectileWeapon::Function_ShootWeaponTrace()
{
	// Placeholder code to setup trace start point and direction .................................................................................||
	_TraceStartPoint = GetActorLocation() + GetActorForwardVector() * Ph_StartPointOffset; // WIP, this should come from a Socket on the weapon mesh
	_TraceDirection = GetActorForwardVector();

	// Debug message to indicate method call .................................
	if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::White, GetName() + " | AProjectileWeapon::Method_ShootTrace()"); }

	// Setup trace parameters ................................................
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	// Perform line trace......................................................
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		_TraceHitResult,									// Out hit result
		_TraceStartPoint,									// Start point
		_TraceStartPoint + (_TraceDirection * _TraceRange), // End point
		ECC_Visibility,										// Trace channel
		TraceParams											// Query parameters
	);

	// Debug line to visualize the trace ......................................
	DrawDebugLine(
		GetWorld(),
		_TraceStartPoint, // Start point
		_TraceStartPoint + (_TraceDirection * _TraceRange), // End point
		FColor::Red, // Color
		false, // bPersistentLines
		-1, // LifeTime
		1, // DepthPriority
		1.0f // Thickness
	);
}

void AProjectileWeapon::Method_SetProjectileDestinationPoint()
{
	// W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P | W I P |
	// If it has no impact point than set the destination point to center of the screen direction * range
	//...
	FVector DestinationPoint = _TraceHitResult.bBlockingHit ? _TraceHitResult.ImpactPoint : (_TraceStartPoint + (_TraceDirection * _TraceRange));

	_TraceHitResult.bBlockingHit ?
		// Debug stuff...
		DrawDebugSphere(
			GetWorld(),
			DestinationPoint,
			15.0f, // Radius
			12, // Segments
			FColor::Emerald,
			false, // bPersistentLines
			5.0f, // LifeTime
			2, // DepthPriority
			1.0f) // Thickness
		:
		DrawDebugSphere(
			GetWorld(),
			DestinationPoint,
			15.0f, // Radius
			12, // Segments
			FColor::Red,
			false, // bPersistentLines
			5.0f, // LifeTime
			2, // DepthPriority
			1.0f); // Thickness
}

void AProjectileWeapon::Method_SpawnProjectile()
{
	//Check if it has valid projectile class to spawn .........................................................................................................................
	if(!_ProjectileClass)
	{
		if(GEngine  && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("AProjectileWeapon::Method_SpawnProjectile() - Projectile class is not set!")); }
		return;
	}

	// Get world context ......................................................................................................................................................
	UWorld* Lc_World = GetWorld();
	if (!Lc_World) {
		if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("AProjectileWeapon::Method_SpawnProjectile() - World context is invalid!")); }
		return;
	}

	FVector SpawnLocation = _TraceStartPoint;
	FRotator SpawnRotation = _ProjectileDirection.Rotation(); // WIP / by ChatGPT, needs testing if this will set the rotation correctly so it faces the right direction

	// Debug Arrow to visualize the projectile spawn direction ................................................................................................................
	DrawDebugDirectionalArrow(
		GetWorld(),
		SpawnLocation, // Start
		SpawnLocation + (_ProjectileDirection * 100.0f), // End
		100.0f, // ArrowSize
		FColor::Blue,
		false, // bPersistentLines
		10, // LifeTime
		0, // DepthPriority
		5.0f // Thickness
	);

	
}
