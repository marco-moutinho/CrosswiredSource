// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponUser.h"

#include "WeaponDefinitionPDA.h"
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
	// Check it
	if (OwningActorPtr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, "Owner: " + OwningActorPtr->GetName()); }
		//UActorComponent* L_CamComp = OwningActor->GetComponentByClass<UCameraComponent>();
		CameraComponentPtr = OwningActorPtr->GetComponentByClass<UCameraComponent>();
		if (!CameraComponentPtr)
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, this->GetName() + "UWeaponUser::Function_SetCameraComponentPointer() - Can´t Get UCameraComponent of owner;"); }
		}
		else
		{
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, this->GetName() + "UWeaponUser::Function_SetCameraComponentPointer() - " + CameraComponentPtr->GetName()); }
		}
	}
	else
	{
		return;
	}
}

void UWeaponUser::Function_ManualSetCameraComponentPointer(UCameraComponent* InPointer)
{
	CameraComponentPtr = InPointer;
}

void UWeaponUser::Function_SpawnWeaponFromDefinition(UWeaponDefinitionPDA* InWeaponDef, AWeaponBase*& OutSpawnedWeaponPtr)
{
	/// 1st - Safety checks
	if (InWeaponDef == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - InWeaponDef is null"); }
				return;
	}

	if(OwningActorPtr == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - OwningActorPtr is null"); }
				return;
	}

	if(InWeaponDef->WeaponSoftClassPtr == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - InWeaponDef->WeaponSoftClassPtr is null"); }
				return;
	}

	/// 2nd - Load and Spawn the weapon actor class
	// Load the weapon class from the definition
	TSubclassOf<AWeaponBase> L_WeaponClass = InWeaponDef->WeaponSoftClassPtr.LoadSynchronous();
	
	if(L_WeaponClass == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - Loaded Weapon Class is null"); }
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

	if(L_WeaponPtr == nullptr) // Safety check
	{
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_SpawnWeaponFromDefinition - Spawned Weapon is null"); }
				return;
	}

	/// 5th - Initialize the weapon from the definition
	L_WeaponPtr->Function_InitializeFromDefinition(InWeaponDef);

	/// 6th - Finish Spawning the weapon actor
	UGameplayStatics::FinishSpawningActor(L_WeaponPtr, FTransform::Identity);

	// Set Output pointer
	OutSpawnedWeaponPtr = L_WeaponPtr;

	/* Note about Spawning Actors Deferred vs Instant / Normal:
	* SpawnActor (instant)			  : Constructor → BeginPlay → Your code
	* SpawnActorDeferred (controlled) : Constructor → Your code → FinishSpawning → BeginPlay
	*/
}

void UWeaponUser::Function_AttachWeaponToHands(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent)
{
	// Safety checks
	if (InWeaponRef == nullptr) {
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_AttachWeaponToHands - InWeaponRef is null"); }
				return;
	}
	if (InSceneComponent == nullptr) {
				if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - UWeaponUser::Function_AttachWeaponToHands - InSceneComponent is null"); }
				return;
	}

	// Attach the weapon to the specified scene component
	InWeaponRef->AttachToComponent(InSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UWeaponUser::Function_UnequipCurrentWeapon()
{
}

void UWeaponUser::Function_UseCurrentWeapon()
{
	if(CurrentWeaponPtr)
	{
		CurrentWeaponPtr->Function_StartUse();
	}
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

