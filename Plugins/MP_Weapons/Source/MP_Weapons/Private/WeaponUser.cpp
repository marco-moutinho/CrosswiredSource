// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponUser.h"

#include "WeaponDefinitionPDA.h"
#include "Camera/CameraComponent.h"

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
	OwningActor = GetOwner();
	if (OwningActor)
	{
		if (GEngine)
		{
			FString _1msg = FString::Printf(TEXT("%s UWeaponUser::BeginPlay() - %s"), *this->GetName(), *OwningActor->GetName());
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
	if (OwningActor)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, "Owner: " + OwningActor->GetName()); }
		//UActorComponent* L_CamComp = OwningActor->GetComponentByClass<UCameraComponent>();
		CameraComponentPtr = OwningActor->GetComponentByClass<UCameraComponent>();
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

void UWeaponUser::Function_EquipCurrentWeapon(AWeaponBase* InWeaponRef)
{
	// Set the current weapon pointer;
	CurrentWeaponPtr = InWeaponRef;

	// Attach the weapon to the character's mesh at the appropriate socket
	//... (Attachment logic goes here)
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
	if (CameraComponentPtr == nullptr || OwningActor == nullptr)
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
	L_QueryParams.AddIgnoredActor(OwningActor);

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

