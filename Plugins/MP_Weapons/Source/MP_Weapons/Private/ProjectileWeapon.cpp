// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "WeaponPDA.h"
#include "ProjectileWeaponPDA.h"
#include "WeaponDefinitionPDA.h"

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

void AProjectileWeapon::Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA)
{
	/// WeaponDefinitionPDA
	/// |-> TSoftClassPtr<AWeaponBase> WeaponSoftClassPtr;
	/// |-> TSoftObjectPtr<UTexture2D> WeaponIcon;
	/// |-> TSoftObjectPtr<UWeaponPDA> WeaponDataPDA;
	///	|			       |--> UProjectileWeaponPDA ---> I need to reach here
	/// 
	/// UWeaponPDA > UProjectileWeaponPDA
	
	/*
	* [ NOTE on UE C++ ]
	* .IsNull()          - Was it assigned?
	* .IsValid()         - Is it loaded?
	* .LoadSynchronous() - Loads the asset if not loaded and returns the asset pointer
	* .Get()             - Only use after loaded.
	*/

	/*
	* Simple mental rule
	* "Do I only need to know the type?" → IsA 
	+ "Do I need to use subclass data?" → Cast
	*/

	// Call parent method
	Super::Function_InitializeFromDefinition(WeaponDefinitionPDA);

	// Safety check : Check for valid Weapon Definition PDA
	if(!IsValid(WeaponDefinitionPDA))
	{
		if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Invalid Weapon Definition PDA!")); }
		return;
	}

	// Satety check : Check if the WeaponDataPDA was set on Editor
	if (WeaponDefinitionPDA->WeaponDataPDA.IsNull())
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - WeaponDataPDA is null! / Was Not set on Editor")); }
		return;
	}

	// Load the Weapon Data PDA synchronously
	UWeaponPDA* L_WeaponDataPDA = WeaponDefinitionPDA->WeaponDataPDA.LoadSynchronous();

	// Safety check : Check if L_WeaponDataPDA was loaded successfully
	if (!IsValid(L_WeaponDataPDA))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Failed to load WeaponDataPDA!")); }
		return;
	}

	// Attempt to cast to Projectile Weapon PDA
	const UProjectileWeaponPDA* L_ProjectileWeaponDefPDA = Cast<UProjectileWeaponPDA>(L_WeaponDataPDA);

	// Safety check : Check for valid cast
	if (!L_ProjectileWeaponDefPDA)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Invalid Projectile Weapon Definition PDA!")); }
		return;
	}

	// Add On Screen Debug Message to indicate successful cast
	if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Successfully casted to Projectile Weapon Definition PDA!")); }

	//// Initialize Projectile Weapon specific properties from the PDA
	//_TraceRange = L_ProjectileWeaponDefPDA->WeaponTraceRange;
	//_WeaponFireRate = L_ProjectileWeaponDefPDA->WeaponFireRate;
	//_ProjectileClass = L_ProjectileWeaponDefPDA->ProjectileClassPtr.LoadSynchronous();
}

// WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP |
void AProjectileWeapon::Function_ShootWeaponTrace()
{
	// Placeholder code to setup trace start point and direction
	_TraceStartPoint = GetActorLocation() + GetActorRotation().RotateVector(Ph_StartPointOffset); // WIP, this should come from a Socket on the weapon mesh
	_TraceDirection = GetActorForwardVector();

	// Setup trace parameters
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	// Perform line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		_TraceHitResult,									// Out hit result
		_TraceStartPoint,									// Start point
		_TraceStartPoint + (_TraceDirection * _TraceRange), // End point
		ECC_Visibility,										// Trace channel
		TraceParams											// Query parameters
	);

	// Debug line to visualize the trace
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

	DrawDebugPoint( GetWorld(), _TraceStartPoint, 10, FColor::Blue, false, -1, 1);
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
	//Check if it has valid projectile class to spawn
	if(!_ProjectileClass)
	{
		if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("AProjectileWeapon::Method_SpawnProjectile() - Projectile class is not set!")); }
		return;
	}

	// Get world context
	UWorld* Lc_World = GetWorld();

	// Safety check for valid world context
	if (!Lc_World)
	{
		// [ Waring Message ]
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("AProjectileWeapon::Method_SpawnProjectile() - World context is invalid!")); }
		return;
	}

	// Setup spawn parameters

	FVector SpawnLocation = _TraceStartPoint;
	FRotator SpawnRotation = _ProjectileDirection.Rotation(); // WIP / by ChatGPT, needs testing if this will set the rotation correctly so it faces the right direction

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn the projectile actor
	AProjectileBase* SpawnedProjectile = Lc_World->SpawnActor<AProjectileBase>(
		_ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (bDebugMode)
	{
		// Debug Arrow to visualize the projectile spawn direction
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
	
}

void AProjectileWeapon::Function_LoadProjectileClass()
{
}

void AProjectileWeapon::Function_InitializeProjectileSubClassData()
{
}
