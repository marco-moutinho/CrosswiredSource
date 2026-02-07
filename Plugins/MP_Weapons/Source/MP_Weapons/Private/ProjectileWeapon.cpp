// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "WeaponPDA.h"
#include "ProjectileWeaponPDA.h"
#include "WeaponDefinitionPDA.h"

#include "GameFramework/Character.h"
#include "PoolComponent.h"

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
	PoolComponentPtr = CreateDefaultSubobject<UPoolComponent>(TEXT("PoolComponent"));

}

void AProjectileWeapon::BeginPlay()
{
	// Call parent BeginPlay
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("AProjectileWeapon::BeginPlay()"));
	this->Function_InitialSetup();
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
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::Function_InitializeFromDefinition()"));
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

	// Safety check : Check if L_WeaponDataPDA was loaded successfully
	if (!IsValid(this->WeaponData))
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Failed to load WeaponDataPDA!")); }
		return;
	}

	// OK after all the safety checks of base class and input parameters been validated STORE the ProjectileWeaponPDA ( DATA REF ) on this Weapon Actor
	// Attempt to cast to Projectile Weapon PDA ( cause ProjectileWeaponPDA is a subclass of WeaponPDA, and it is needed the specifics about the projectile sub
	ProjectileWeaponDataPtr = Cast<UProjectileWeaponPDA>(WeaponData);

	// Safety check : Check for valid cast
	if (ProjectileWeaponDataPtr == nullptr){
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + TEXT("AProjectileWeapon::Function_InitializeFromDefinition() - Invalid Projectile Weapon Definition PDA!")); }
		return;
	}
	// Safety check : if the Projectile (softRef) was set on the editor;
	if (ProjectileWeaponDataPtr->ProjectileSoftClassPtr.IsNull()) {
		GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "ProjectileWeaponDataPtr->ProjectileSoftClassPtr.IsNull()"); return;
	}
	ProjectileSoftClassPtr = ProjectileWeaponDataPtr->ProjectileSoftClassPtr.LoadSynchronous();
	// Set the initial projectile class
	//TSubclassOf<AProjectileBase> LcProjectileSubClass = Function_LoadProjectileClass(ProjectileWeaponDataPtr->ProjectileSoftClassPtr.LoadSynchronous()); // so the problem is that the input is null, and i know that i assign it on the editor
	_ProjectileSubClassRef = ProjectileWeaponDataPtr->ProjectileSoftClassPtr.LoadSynchronous();
	if (_ProjectileSubClassRef == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "::Function_InitializeFromDefinition - SHIIITFailed to LoadProjectileClass()"); } return; }
	Function_SetProjectileClass(_ProjectileSubClassRef);

	//this->Function_InitialSetup();
}

void AProjectileWeapon::FunctionInitializeProjectilePool()
{
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::FunctionInitializeProjectilePool()"));
	if (_ProjectileSubClassRef == nullptr) { UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::FunctionInitializeProjectilePool() - _ProjectileSubClassRef.Get()==false")); }
	// Function_InitializePool() uses TSubclassOf<AActor> as input parameter but this->ProjectileClassPtr is 
	PoolComponentPtr->Function_InitializePool(_ProjectileSubClassRef, 6, EPoolState::SleepSt);
}

void AProjectileWeapon::Function_ExecuteWeaponAction()
{
	Function_Shoot();
}

void AProjectileWeapon::Function_InitialSetup()
{
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::Function_InitialSetup()"));
	if (ProjectileWeaponDataPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileWeaponDataPtr valid: %s"), *ProjectileWeaponDataPtr->GetName());
		if (ProjectileWeaponDataPtr->ProjectileSoftClassPtr.IsNull()) {
		UE_LOG(LogTemp, Error, TEXT("ProjectileSoftClassPtr is NULL!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ProjectileSoftClassPtr path: %s"), *ProjectileWeaponDataPtr->ProjectileSoftClassPtr.ToString());
		}
	}
	// Safety Check
	if (_ProjectileSubClassRef == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "::Function_InitialSetup() - _ProjectileSubClassPtr = nullptr"); } return; }
	
	// fill pool
	if(PoolComponentPtr == nullptr) { UE_LOG(LogTemp, Error, TEXT("PoolComponentPtr = nullptr")); }
	PoolComponentPtr->Function_InitializePool(_ProjectileSubClassRef, _ProjectileAmmount, EPoolState::SleepSt);
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

FTransform AProjectileWeapon::Function_SetProjectileSpawnTransform()
{
	FVector SpawnLocation = _TraceStartPoint;
	FQuat SpawnRotator = OwningPawnPtr->GetController()->GetControlRotation().Quaternion();

	FTransform LcTransform;
	LcTransform.SetLocation(SpawnLocation);
	LcTransform.SetRotation(SpawnRotator);

	return LcTransform;
}

TSubclassOf<AProjectileBase> AProjectileWeapon::Function_LoadProjectileClass(TSoftClassPtr<AProjectileBase> InSoftProjectileClassPtr)
{
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::Function_LoadProjectileClass()"));
	// Safety check : is the input parameter valid? if not return
	if (InSoftProjectileClassPtr.IsNull()) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "AProjectileWeapon::Function_LoadProjectileClass - InSoftProjectileClassPtr.IsNull"); }
		return nullptr;
	}

	// Resolve / load the InSoftProjectileClassPtr to a hard ref and set it as the output parameter
	TSubclassOf<AProjectileBase> LcClassHardPtr = InSoftProjectileClassPtr.LoadSynchronous();

	// Safety check : if the projectile hard ref was set / loaded
	if (!LcClassHardPtr.Get()) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "AProjectileWeapon::Function_LoadProjectileClass - OutProjectileClassRef.Get() has return a nullptr"); }
		return nullptr;
	}

	return LcClassHardPtr;

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
	/// Call                          | Loads Asset?     | Retuns 
	/// ---------------------|-----------------|---------------------
	/// Get()                        | No                    | UClass* or nullptr       | .Get() is usually only needed if you need the raw pointer for passing into APIs expecting UClass*.
	/// LoadSynchronous()  | Yes (blocking)   | UClass*
	/// IsNull()                     | No                    | Checks if path exists
}

void AProjectileWeapon::Function_InitializeProjectileSubClassData()
{
}

void AProjectileWeapon::Function_SetProjectileClass(TSubclassOf<AProjectileBase> InProjectileClass)
{
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::Function_SetProjectileClass()"));
	// Safety check : the input parameter class is exist? if not return
	if (InProjectileClass == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "AProjectileWeapon::Function_SetProjectileClass() - InProjectileClass failed to be valid!"); }
		return;
	}
	// Set the projectile class
	_ProjectileSubClassRef = InProjectileClass;

	// TO DO : Problably good idea do give some feedback to the player, maybe through the function/class that calls this one
}

void AProjectileWeapon::Function_Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("AProjectileWeapon::Function_Shoot()"));
	// First acquire a projectile actor // new pool component
	AActor* LcActorPtr = PoolComponentPtr->Function_AcquireActorPtr(); 
	 //Safety check
	if (LcActorPtr == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_Shoot() - LcActorPtr = nullptr"); } return; } // this trigger when i shoot
	// safety check if implements the ProjectileInterface to ensure that the actor is in fact a projectile
	if (LcActorPtr->GetClass()->ImplementsInterface(UProjectileInterface::StaticClass())) {
		IProjectileInterface::Execute_IFunction_LaunchProjectile(LcActorPtr, this->Function_SetProjectileSpawnTransform());
	}
}
