// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpStartCharacter.h"

// should this include be on the .h file or can be on this file 100% problem free?
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJumpStartCharacter::AJumpStartCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Initialize and attach the character core components ----------------------------------------------------------------

	//Initialize Spring arm
	M_SpringArm_Ptr = CreateDefaultSubobject<USpringArmComponent>(TEXT("QS Spring Arm Component"));
	// Attach it
	M_SpringArm_Ptr->SetupAttachment(RootComponent);

	// Initialize Camera
	M_Camera_Ptr = CreateDefaultSubobject<UCameraComponent>(TEXT("QS Camera Component"));
	// Attach it
	M_Camera_Ptr->SetupAttachment(M_SpringArm_Ptr);

}

// Called when the game starts or when spawned
void AJumpStartCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJumpStartCharacter::Function_SetThirdPersonControlSettings()
{
	bUseControllerRotationYaw = false;
	M_SpringArm_Ptr->bUsePawnControlRotation = true;
	M_Camera_Ptr->bUsePawnControlRotation = false;
	this->GetCharacterMovement()->bOrientRotationToMovement = true; // if false it can "strafe"
}

void AJumpStartCharacter::Function_SetFirstPersonControlSettings()
{
	bUseControllerRotationYaw = true;
	M_SpringArm_Ptr->bUsePawnControlRotation = true;
	M_Camera_Ptr->bUsePawnControlRotation = true;
	this->GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called every frame
void AJumpStartCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJumpStartCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJumpStartCharacter::Function_MoveSimple(FVector2D In_Direction)
{
	// MEMORY NOTES: The var "Controller" is a Built-in on Character class,
	// even I had made this method thinking to be called from a PlayerController, I had decided to check this cause someone called from the character itself or something
	if (Controller != nullptr)
	{
		// Get Controller Rotation | create Rotator var and set it´s values
		FRotator L_ControlRotation = Controller->GetControlRotation();

		// We only care about yaw
		FRotator L_YawRotation(0, L_ControlRotation.Yaw, 0);

		// Get forward and right vectors
		const FVector L_ForwardDir = FRotationMatrix(L_YawRotation).GetUnitAxis(EAxis::X);
		const FVector L_RightDir = FRotationMatrix(L_YawRotation).GetUnitAxis(EAxis::Y);

		if (In_Direction.Y != 0)
		{
			AddMovementInput(L_ForwardDir, In_Direction.Y);
		}
		if (In_Direction.X != 0)
		{
			AddMovementInput(L_RightDir, In_Direction.X);
		}
	}
}

void AJumpStartCharacter::Function_LookSimple(FVector2D In_Direction, float In_Sensitivity, bool In_IsInverted)
{
	if (Controller != nullptr)
	{
		// Calculate the look vector based on the player input and sensitivity
		FVector2D L_LookVector(In_Direction * In_Sensitivity);

		// Apply Yaw (Look right/left)
		if (In_Direction.X != 0) { AddControllerYawInput(L_LookVector.X); }

		// Apply Pitch (Look up/down)
		if (In_Direction.Y != 0)if (In_IsInverted) { L_LookVector.Y = -L_LookVector.Y; }
		AddControllerPitchInput(L_LookVector.Y);
	}
}

void AJumpStartCharacter::Function_TraceFromPOV()
{
	FVector L_TraceStart = M_Camera_Ptr->GetComponentLocation();
	FVector L_TraceEnd = M_Camera_Ptr->GetComponentLocation() + (M_Camera_Ptr->GetForwardVector() * _POVTraceLenght);

	// NOTE : Maybe I should "bake" this insted of calculate/set every call
	FCollisionQueryParams L_QueryParms;
	L_QueryParms.AddIgnoredActor(this);

	// Trace
	bool L_bHit = GetWorld()->SweepSingleByProfile(
		M_POVTracehitResult,
		L_TraceStart,
		L_TraceEnd,
		FQuat::Identity,
		_POVTraceProfile.Name,
		FCollisionShape::MakeSphere(_POVSphereTraceRadius),
		L_QueryParms
	);
#if WITH_EDITOR // DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY | DEBUG EDITOR ONLY |
	// Debug Start
	DrawDebugSphere(
		GetWorld(),
		L_TraceStart,
		_POVSphereTraceRadius,
		1,
		FColor::White,
		false,
		1
	);
	// Debug Draw
	if (L_bHit) {
		if (M_DebugMode && M_ShowDebugDraws) {
			// True
			DrawDebugSphere(
				GetWorld(),
				M_POVTracehitResult.ImpactPoint,
				_POVSphereTraceRadius,
				30,
				FColor::White,
				false,
				1,
				-1,
				0.3f);
			// Parameters : World, start, radius, segments , Color, persistance, duration, depth, thickness
			DrawDebugSphere(GetWorld(), M_POVTracehitResult.ImpactPoint, 5, 12, FColor::Black, false, 1, 1, 0.3);
			// Parameters : World, start, arrowsize , Color, persistance, duration, depth, thickness
			DrawDebugDirectionalArrow(GetWorld(), L_TraceStart, M_POVTracehitResult.ImpactPoint, 100, FColor::Black, false, 1, 1, 0.3f);
			DrawDebugDirectionalArrow(GetWorld(), L_TraceStart, L_TraceEnd, 100, FColor::White, false, 1, 1, 0.3f);
		}
		
	}
	else
	{
		if (M_DebugMode && M_ShowDebugDraws){
			DrawDebugSphere(
				GetWorld(),
				L_TraceEnd,
				_POVSphereTraceRadius,
				30,
				FColor::White,
				false,
				0.5f, // duration
				1, // depth
				0.2); // thickness
		}
	}
#endif // .................................................................................................................................................................................................
}


