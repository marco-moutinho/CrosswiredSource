// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponUser.h"

#include "WeaponDefinitionPDA.h"
#include "ProjectileWeaponPDA.h"
#include "WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeaponUser::UWeaponUser()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false); // Disabled by default, can be enabled later on
	// ...
}


// Called when the game starts
void UWeaponUser::BeginPlay()
{
	Super::BeginPlay();

	// Set OwningActor ptr
	OwningActorPtr = GetOwner();
	if (OwningActorPtr)
	{
		if (GEngine)
		{
			FString _1msg = FString::Printf(TEXT("%s UWeaponUser::BeginPlay() - %s"), *this->GetName(), *OwningActorPtr->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, _1msg);

		}
	}
	else { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + " UWeaponUser::BeginPlay() - OwningActorPtr is null"); } }

	// Debug the setup automation
	FString msg = FString::Printf(TEXT("[ %s ] UWeaponUser::BeginPlay() - bAutomatic setup is %s"), *this->GetName(), *LexToString(bAutomaticSetup));
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 20, FColor::White, msg); }
	
	// perform the automatic setup functions calls
	if (bAutomaticSetup)
	{
		Function_AutoSetCameraComponentPointer();
	}
	// ...
	
}


// Called every frame
void UWeaponUser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponUser::Function_AutoSetCameraComponentPointer()
{
	// Check if Owning Actor ptr is valid
	if (OwningActorPtr)
	{
		//UActorComponent* L_CamComp = OwningActor->GetComponentByClass<UCameraComponent>();
		CameraComponentPtr = OwningActorPtr->GetComponentByClass<UCameraComponent>();
		
		// [Warning] Debug  message if CameraComponent ptr is null
		if (!CameraComponentPtr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "UWeaponUser::Function_SetCameraComponentPointer() - Can´t Get UCameraComponent of owner;"); } }
	}
	else
	{
		// [Warning] debug message if Owning Actor ptr is null
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + " UWeaponUser::Function_SetCameraComponentPointer() - OwningActorPtr is null;"); }
		return;
	}
}

void UWeaponUser::Function_ManualSetCameraComponentPointer(UCameraComponent* InPointer)
{
	if(InPointer == nullptr)
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_ManualSetCameraComponentPointer - InPointer is null"); }
				return;
	}
	CameraComponentPtr = InPointer;
}

void UWeaponUser::Function_TraceFromPov(FHitResult& OutHitResult)
{
	if (CameraComponentPtr == nullptr || OwningActorPtr == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, "Component Pointer is null"); }
		return;
	}
	// Write trace parameters
	FVector L_TraceStart = CameraComponentPtr->GetComponentLocation();
	FVector L_TraceEnd = L_TraceStart + (CameraComponentPtr->GetForwardVector() * POVTraceRange);
	FQuat L_TraceQuat = CameraComponentPtr->GetComponentRotation().Quaternion();
	FCollisionShape L_Shape = FCollisionShape::MakeSphere(PovTraceSphereRadius);

	FCollisionQueryParams L_QueryParams;
	L_QueryParams.AddIgnoredActor(OwningActorPtr);

	// perfrom trace
	bool bHit = GetWorld()->SweepSingleByProfile(TraceResult, L_TraceStart, L_TraceEnd, L_TraceQuat, PovTraceProfile.Name, L_Shape, L_QueryParams);
	OutHitResult = TraceResult;


	if (bDebug_EnableTraceDraws)
	{
		// Debug Hit Point
		if (bHit)
		{
			DrawDebugPoint(GetWorld(), TraceResult.ImpactPoint, Debug_HitPointSize, Debug_TraceHitColor, false, Debug_DrawTime, Debug_SphereStartDepth);
		}

		// debug draws...
		// Trace Arrow
		DrawDebugDirectionalArrow(GetWorld(), L_TraceStart, L_TraceEnd, 300, Debug_TraceArrowColor, false, Debug_DrawTime, Debug_ArrowDepth, Debug_ArrowThickness);
		// Trace Start Sphere
		DrawDebugSphere(GetWorld(), L_TraceStart, PovTraceSphereRadius, Debug_SphereSegments, Debug_TraceEndColor, false, Debug_DrawTime, Debug_SphereStartDepth, Debug_SphereStartThickness);
		// Trace End
		DrawDebugSphere(GetWorld(), L_TraceEnd, PovTraceSphereRadius, Debug_SphereSegments, Debug_TraceEndColor, false, Debug_DrawTime, Debug_SphereStartDepth, Debug_SphereStartThickness);
		//Trace sweep
		for (int i = 0; i <= Debug_SphereSweepTimes; i++) {
			float Alpha = (float)i / Debug_SphereSweepTimes;
			FVector Pos = FMath::Lerp(L_TraceStart, L_TraceEnd, Alpha);
			DrawDebugSphere(GetWorld(), Pos, PovTraceSphereRadius, Debug_SphereSegments, Debug_TraceSweepColor, false, Debug_DrawTime, Debug_SphereSweepDepth, Debug_SphereSweepThickness);
		}
		// TO DO: Calculate proper sweep interval based on trace distance and sphere radius
	}
}

void UWeaponUser::Function_EquipWeaponAdvanced(int InHolsterSlotIndex, USceneComponent* InSceneComponent, AWeaponBase*& OutWeapon)
{
	// Spawn
	AWeaponBase* L_WeaponPtr;
	this->Function_SpawnWeaponFromHolster(InHolsterSlotIndex, L_WeaponPtr);
	
	if (L_WeaponPtr == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_EquipWeaponAdvanced - L_WeaponPtr is null"); }
		return;
	}

	//Equip
	this->Function_EquipWeapon(L_WeaponPtr, InSceneComponent);

	// Update Holster / aka Weapon Wheel "Data State"
	Holster[InHolsterSlotIndex].WeaponSpawnedPtr = L_WeaponPtr;

	OutWeapon = L_WeaponPtr;
}

void UWeaponUser::Function_SpawnWeaponFromHolster(int InSlotIndex, AWeaponBase*& OutWeaponPtr)
{
	// Safety check : for valid slot index
	if(!Holster.IsValidIndex(InSlotIndex))
	{
		if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SetCurrentWeaponFromSlot - InSlotIndex is invalid"); }
		OutWeaponPtr = nullptr;
		return;
	}

	// Safety check : Check if the Definition was assigned eg. in editor
	if(Holster[InSlotIndex].WeaponDefinition.IsNull())
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SetCurrentWeaponFromSlot - Weapon Definition at index is null, problably wasn't assign on Editor;"); }
		OutWeaponPtr = nullptr;
		return;
	}
	
	// is it worth checking if the WeaponDefinition is already loaded? LoadSynchronous should handle that anyway...
	// Get the weapon definition from the holdster based on the provided slot index
	UWeaponDefinitionPDA* L_WeaponDef = Holster[InSlotIndex].WeaponDefinition.LoadSynchronous();
	
	// Safety check for valid weapon definition ( if the index returned a null ptr )
	if (L_WeaponDef == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SetCurrentWeaponFromSlot - Weapon Definition at index is null"); }
		OutWeaponPtr = nullptr;
		return;
	}

	// Check if the Weapon Actor is already spawned ( to avoid spawning multiple instances of the same weapon definition )
	if (IsValid(Holster[InSlotIndex].WeaponSpawnedPtr))
	{
		OutWeaponPtr = Holster[InSlotIndex].WeaponSpawnedPtr;
		return;
	}

	// Set the current weapon based on the weapon definition
	AWeaponBase* L_OutWeapon;
	this->Function_SpawnWeaponFromDefinition(L_WeaponDef, L_OutWeapon); // this is only needed if you want to spawn from definition, otherwise you can pass an already spawned weapon actor
	
	// safety check
	if (L_OutWeapon == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SetCurrentWeaponFromSlot - L_OutWeapon is null"); }
		OutWeaponPtr = nullptr;
		return;
	}

	OutWeaponPtr = L_OutWeapon;
}

void UWeaponUser::Function_SpawnWeaponFromDefinition(UWeaponDefinitionPDA* InWeaponDef, AWeaponBase*& OutSpawnedWeaponPtr)
{
	/// 1st - Safety checks
	// Check if the Weapon Definition PDA is valid
	if (InWeaponDef == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - InWeaponDef is null"); }
				OutSpawnedWeaponPtr = nullptr;
				return;
	}

	// Check if the WeaponSoftClassPtr is set in the definition
	if(InWeaponDef->WeaponSoftClassPtr.IsNull()) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - InWeaponDef->WeaponSoftClassPtr is null"); }
				OutSpawnedWeaponPtr = nullptr;
				return;
	}

	// Check if the Owning Actor ptr is valid
	if (OwningActorPtr == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - OwningActorPtr is null"); }
		OutSpawnedWeaponPtr = nullptr;
		return;
	}

	/// 2nd - Load and Spawn the weapon actor class
	// Load the weapon class from the definition ( if not already loaded )
	TSubclassOf<AWeaponBase> L_WeaponClass = InWeaponDef->WeaponSoftClassPtr.LoadSynchronous();
	
	// Safety check : Check if the loaded weapon class is valid
	if(L_WeaponClass == nullptr)
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - Loaded Weapon Class is null"); }
				OutSpawnedWeaponPtr = nullptr;
				return;
	}

	/// 3rd - Prepare Spawn Parameters
	// Spawn Parameters
	FActorSpawnParameters L_SpawnParams;
	L_SpawnParams.Owner = OwningActorPtr;
	L_SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	/// 4th - Spawn the weapon actor deferred
	// Spawn the weapon actor
	AWeaponBase* L_WeaponPtr = GetWorld()->SpawnActorDeferred<AWeaponBase>(L_WeaponClass, FTransform::Identity, OwningActorPtr, nullptr, L_SpawnParams.SpawnCollisionHandlingOverride);

	// Safety check : Check if the spawned weapon actor is valid
	if(L_WeaponPtr == nullptr)
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - Spawned Weapon is null"); }
				OutSpawnedWeaponPtr = nullptr;
				return;
	}

	/// 5th - Initialize the weapon from the definition
	L_WeaponPtr->Function_InitializeFromDefinition(InWeaponDef);

	/// 6h - Initialize external ptrs
	if (OwningActorPtr->IsA(APawn::StaticClass())) {
		OwningPawnPtr = CastChecked<APawn>(GetOwner());
	}
	else
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "UWeaponUser::Function_SpawnWeaponFromDefinition - OwningActor is not a Pawn or subclass of it; !"); }
		return;
	}
	L_WeaponPtr->Function_InitializeExternalPointers(OwningPawnPtr);

	/// 7th - Finish Spawning the weapon actor
	// This is the line that the weapon actor physicly spawns on the world
	UGameplayStatics::FinishSpawningActor(L_WeaponPtr, FTransform::Identity);

	// Set Output pointer
	OutSpawnedWeaponPtr = L_WeaponPtr;

	/*
	* Note about Spawning Actors Deferred vs Instant / Normal:
	* SpawnActor (instant)			  : Constructor → BeginPlay → Your code
	* SpawnActorDeferred (controlled) : Constructor → Your code → FinishSpawning → BeginPlay
	*/
}

void UWeaponUser::Function_EquipWeapon(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent)
{
	// Safety check
	if (InWeaponRef == nullptr)
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_EquipWeapon - InWeaponRef is null"); }
				return;
	}

	// Set the current weapon pointer to the provided weapon reference
	CurrentWeaponPtr = InWeaponRef;

	// Attach the weapon to the hands
	this->Function_AttachWeaponToHands(CurrentWeaponPtr, InSceneComponent);
}

void UWeaponUser::Function_AttachWeaponToHands(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent)
{
	// Safety checks
	if (InWeaponRef == nullptr)
	{
		if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_AttachWeaponToHands - InWeaponRef is null"); }
		return;
	}
	if (InSceneComponent == nullptr)
	{
		// Uncomment this to enforce providing a scene component
		/*if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_AttachWeaponToHands - InSceneComponent is null"); }
		return;*/

		// If no scene component was provided, use the default/Placeholder weapon attach point
		InSceneComponent = PH_WeaponAttachPoint;
	}

	// Attach the weapon to the specified scene component
	InWeaponRef->AttachToComponent(InSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UWeaponUser::Function_UseCurrentWeapon(EInputphase InInputphase)
{
	// Safety check
	if(CurrentWeaponPtr == nullptr)
	{
		// [Warning] debug message if CurrentWeaponPtr is null
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_UseCurrentWeapon - CurrentWeaponPtr is null"); }
		return;
	}

	// Run a switch on the input phase
	switch (InInputphase)
	{
	case EInputphase::IP_OnStartPress:
		CurrentWeaponPtr->Function_ExecuteWeaponAction();
		break;
	case EInputphase::IP_OnBeingHeld:
		break;
	case EInputphase::IP_OnBeingReleased:
		break;
	}
}

void UWeaponUser::Function_UnequipCurrentWeapon(EWeaponUnquipMode InUWMode)
{
	// Safety Check : Check if CurrentWeaponPtr is valid
	if (CurrentWeaponPtr == nullptr)
	{
		// [Warning] debug message if CurrentWeaponPtr is null
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_UnequipCurrentWeapon - CurrentWeaponPtr is null"); }
		return;
	}

	switch (InUWMode)
	{
	case EWeaponUnquipMode::EWUM_ToHolster:
		this->Function_StoreCurrentWeaponInHolster();
		break;
	case EWeaponUnquipMode::EWUM_DropOnGround:
		break;
	case EWeaponUnquipMode::EWUM_Destroy:
		break;

	default:
		break;
	}
}

void UWeaponUser::Function_StoreCurrentWeaponInHolster()
{
	// Safety Check : Check if CurrentWeaponPtr is valid
	if (CurrentWeaponPtr == nullptr)
	{
		// [Warning] debug message if CurrentWeaponPtr is null
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_StoreCurrentWeaponInHolster - CurrentWeaponPtr is null"); }
		return;
	}

	// Whats the corresponding weapon slot on Holdster for the current weapon?
}

