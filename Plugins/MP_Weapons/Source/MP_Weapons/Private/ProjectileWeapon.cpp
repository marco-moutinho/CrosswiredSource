// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "WeaponPDA.h"
#include "ProjectileWeaponPDA.h"
#include "WeaponDefinitionPDA.h"

#include "GameFramework/Character.h"
#include "ObjectPoolComponent.h"
//#include "IPoolableActor.h"

AProjectileWeapon::AProjectileWeapon()
{
	// Call parent constructor -  IP : Is this needed? Need to checkout
	//Super::AWeaponBase();

	// Enable Tick function
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickEnabled(false); // Enabled by default for testing, can be disabled later on

	//Set tick group
	PrimaryActorTick.TickGroup = TG_LastDemotable;

	// Initialize Pool Component
	PoolComponentPtr = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ProjectilePool AComponent"));

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

	// Attempt to cast to Projectile Weapon PDA ( cause ProjectileWeaponPDA is a subclass of WeaponPDA, and it is needed the specifics about the projectile sub
	const UProjectileWeaponPDA* L_ProjectileWeaponDefPDA = Cast<UProjectileWeaponPDA>(L_WeaponDataPDA);

	// Safety check : Check for valid cast
	if (!L_ProjectileWeaponDefPDA)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Invalid Projectile Weapon Definition PDA!")); }
		return;
	}

	// Set the initial projectile class
	TSubclassOf<AProjectileBase> L_WeaponProjectilePtr;
	Function_LoadProjectileClass(L_ProjectileWeaponDefPDA->ProjectileClassPtr, L_WeaponProjectilePtr);
	Function_SetProjectileClass(L_WeaponProjectilePtr);

	// Add On Screen Debug Message to indicate successful cast
	if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Successfully casted to Projectile Weapon Definition PDA!")); }

	//// Initialize Projectile Weapon specific properties from the PDA
	//_TraceRange = L_ProjectileWeaponDefPDA->WeaponTraceRange;
	//_WeaponFireRate = L_ProjectileWeaponDefPDA->WeaponFireRate;
	//_ProjectileClass = L_ProjectileWeaponDefPDA->ProjectileClassPtr.LoadSynchronous();
}

void AProjectileWeapon::FunctionInitializeProjectilePool()
{
}

void AProjectileWeapon::Function_ExecuteWeaponAction()
{
	//Method_SpawnProjectile();
}

// WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP | WIP |
void AProjectileWeapon::Function_ShootWeaponTrace()
{
	/* 
	* GetActorRotation().RotateVector(...)
	* Converts a local-space vector into world-space;
	* Without this, the offset would always be applied in world axes (wrong);
	* Ensures the offset moves with weapon rotation
	*/

	// WIP, this should come from a Socket on the weapon mesh
	// Placeholder code to setup trace start point and direction
	_TraceStartPoint = GetActorLocation() + GetActorRotation().RotateVector(Ph_StartPointOffset);
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

void AProjectileWeapon::Method_SetProjectileDestinationPoint(FVector InTargetLocation)
{
}

void AProjectileWeapon::Method_SpawnProjectile()
{
	//Check if it has valid projectile class to spawn
	if(!_ProjectileClassRef)
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

	// Set up Spawn

	// Set spawn position
	FVector SpawnLocation = _TraceStartPoint;
	
	// Safety check
	if (OwningPawnPtr == nullptr) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - AProjectileWeapon::Method_SpawnProjectile() - OwningPawnPtr is a nullptr !"); }
		return;
	}
	// Adictional safety check
	if (OwningPawnPtr->IsA(ACharacter::StaticClass()) == false)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - AProjectileWeapon::Method_SpawnProjectile() - OwningPawnPtr is not a character !"); }
		return;
	}


	// Set Spawn Rotation
	FRotator SpawnRotation;
	// Match as player view/Aim/POV
	SpawnRotation = OwningPawnPtr->GetController()->GetControlRotation(); // is this to heavy processing power to call on each time a projectile its fired/spawn?
	
	// Setup spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn the projectile actor
	AProjectileBase* SpawnedProjectile = Lc_World->SpawnActor<AProjectileBase>(_ProjectileClassRef, SpawnLocation, SpawnRotation, SpawnParams);

	// Add it to the pool
	PoolComponentPtr->Function_AddToPool(SpawnedProjectile, true);

	if (bDebugMode)
	{
		// Debug Arrow to visualize the projectile spawn direction
		DrawDebugDirectionalArrow(
			GetWorld(),
			SpawnLocation, // Start
			SpawnLocation + (SpawnRotation.Vector() * 100), // End location
			100.0f, // ArrowSize
			FColor::Blue,
			false, // bPersistentLines
			0.6f, // LifeTime
			1, // DepthPriority
			1.0f // Thickness
		);

		// Debug a sphere that represents the spawn area/location
		DrawDebugSphere(
			GetWorld(),
			SpawnLocation,
			3, //radius
			6, // segments
			FColor::Black,
			false,
			0.6f, //lifetime
			2, //depth
			1 //thick
		);
	}
	
}

void AProjectileWeapon::Function_LoadProjectileClass(TSoftClassPtr<AProjectileBase> InSoftProjectileClassPtr, TSubclassOf<AProjectileBase>& OutProjectileClassRef)
{
	// Safety check : is the input parameter valid? if not return
	if (InSoftProjectileClassPtr.IsNull()) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "AProjectileWeapon::Function_LoadProjectileClass - InSoftProjectileClassPtr.IsNull"); }
		OutProjectileClassRef = nullptr;
		return;
	}

	// Resolve / load the InSoftProjectileClassPtr to a hard ref and set it as the output parameter
	OutProjectileClassRef = InSoftProjectileClassPtr.LoadSynchronous();

	// Safety check : if the projectile hard ref was set / loaded
	if (!OutProjectileClassRef.Get()) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "AProjectileWeapon::Function_LoadProjectileClass - OutProjectileClassRef.Get() has return a nullptr"); }
		return;
	}

	/// NOTE about ->
	///                            When to use "->"
	/// |-------------------------------------------------------------------|
	/// | Type		         | What it represents              | Use ->?    |
	/// |-------------------------------------------------------------------|
	/// | TSoftClassPtr<T>   | Lazy reference to a class asset | Never      |
	/// | TSubclassOf<T>     | Loaded UClass* constrained to T | Use .Get() |
	/// | UClass*			 | Actual class object             | Yes        |
	/// | AActor* / UObject* | Live instance				   | Yes        |
	/// |-------------------------------------------------------------------|

	/// [ NOTE about .IsValid ]
	/// Problem : IsValid() on TSubclassOf, This is wrong.
	/// Why? IsValid() is for UObject instances, not class wrappers.
	///		When to use?
	///		IsValid(UObject*)
	///		IsValid(AActor*)
	/// 
	/// Correct way to validate a loaded class: "use nullptr check" e.g.:
	/// 
	/// if (!OutProjectileClassRef)                         | implicit check | Exactly the same as if (!OutProjectileClassRef.Get()), just cleaner.
	///
	/// or explicitly:
	/// 
	/// UClass* LoadedClass = OutProjectileClassRef.Get();  | Can be compared to nullptr.| Returns the raw UClass* pointer inside the TSubclassOf.
	///if (!LoadedClass)

	/// [ NOTE about .Get() ]
	/// Returns: UClass* if the asset is already loaded or nullptr if not loaded yet

	/// [ Aditional NOTES ]
	/// Call               | Loads Asset?   | Retuns 
	/// -------------------|----------------|---------------------
	/// Get()              | No             | UClass* or nullptr       | .Get() is usually only needed if you need the raw pointer for passing into APIs expecting UClass*.
	/// LoadSynchronous()  | Yes (blocking) | UClass*
	/// IsNull()           | No             | Checks if path exists
}

void AProjectileWeapon::Function_InitializeProjectileSubClassData()
{
}

void AProjectileWeapon::Function_SetProjectileClass(TSubclassOf<AProjectileBase> InProjectileClass)
{
	// Safety check : the input parameter class is exist? if not return
	if (!IsValid(InProjectileClass) || !InProjectileClass) {
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "AProjectileWeapon::Function_SetProjectileClass() - InProjectileClass failed to be valid!"); }
	}
	// Set the projectile class
	_ProjectileClassRef = InProjectileClass;

	// TO DO : Problably good idea do give some feedback to the player, maybe through the function/class that calls this one
}

void AProjectileWeapon::Function_Shoot()
{
	bool bWasRetrived;
	PoolComponentPtr->Function_RetrieveFromPool(bWasRetrived);
	if (!bWasRetrived) {
		this->Method_SpawnProjectile(); 
	}
}
