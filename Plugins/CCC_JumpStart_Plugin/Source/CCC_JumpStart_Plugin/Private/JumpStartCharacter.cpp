// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpStartCharacter.h"

// should this include be on the .h file or can be on this file 100% problem free?
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
AJumpStartCharacter::AJumpStartCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UJumpStartMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString LcTGName;

	switch (this->PrimaryActorTick.TickGroup)
	{
	case TG_PrePhysics:      LcTGName = TEXT("TG_PrePhysics"); break;
	case TG_StartPhysics:    LcTGName = TEXT("TG_StartPhysics"); break;
	case TG_DuringPhysics:   LcTGName = TEXT("TG_DuringPhysics"); break;
	case TG_PostPhysics:     LcTGName = TEXT("TG_PostPhysics"); break;
	case TG_LastDemotable:   LcTGName = TEXT("TG_LastDemotable"); break;
	case TG_NewlySpawned:    LcTGName = TEXT("TG_NewlySpawned"); break;
	case TG_MAX:             LcTGName = TEXT("TG_MAX"); break;
	default:                 LcTGName = TEXT("Unknown"); break;
	}

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::White, "this tickGroup : " + LcTGName); }


	// Initialize and attach the character core components ----------------------------------------------------------------

	//Initialize Spring arm
	M_SpringArm_Ptr = CreateDefaultSubobject<USpringArmComponent>(TEXT("_QSSpringArmComponent"));
	// Attach it
	M_SpringArm_Ptr->SetupAttachment(RootComponent);
	M_SpringArm_Ptr->SocketOffset = FVector(0, 0, _FirstPersonSpringArmVerticalOffset);
	//M_SpringArm_Ptr->TargetArmLength = _ThirdPersonSpringArmLenght;

	// Initialize Camera
	M_Camera_Ptr = CreateDefaultSubobject<UCameraComponent>(TEXT("_JSCameraComponent"));
	// Attach it
	M_Camera_Ptr->SetupAttachment(M_SpringArm_Ptr);

	// PhysicsHandleComponent
	M_PhysicHandleComponentPtr = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("_PhysicsHandleComponent"));

	// JumpStart CharacterMovementComponent
	M_JSMovementCompPtr = Cast<UJumpStartMovementComponent>(GetCharacterMovement());

}

void AJumpStartCharacter::OnConstruction(const FTransform& Transform)
{
	// Auto initialize the JumpStartCharacter
	//if (_AutoInit) { Function_InitializeJumpStartCharacter(); }
}

void AJumpStartCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Auto initialize the JumpStartCharacter
	if (_AutoInit) { Function_InitializeJumpStartCharacter(); }
}

// Called when the game starts or when spawned
void AJumpStartCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AJumpStartCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast to EnhancedInputComponent
	if (UEnhancedInputComponent* LcEIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Move input
		if (_Inputs.IA_Move) {
			LcEIC->BindAction(_Inputs.IA_Move, ETriggerEvent::Started, this, &AJumpStartCharacter::Function_MoveSimple);
			LcEIC->BindAction(_Inputs.IA_Move, ETriggerEvent::Triggered, this, &AJumpStartCharacter::Function_MoveSimple);
			LcEIC->BindAction(_Inputs.IA_Move, ETriggerEvent::Completed, this, &AJumpStartCharacter::Function_MoveSimple);
			LcEIC->BindAction(_Inputs.IA_Move, ETriggerEvent::Canceled, this, &AJumpStartCharacter::Function_MoveSimple);
		}

		// Look Input
		if (_Inputs.IA_Look) {
			LcEIC->BindAction(_Inputs.IA_Look, ETriggerEvent::Started, this, &AJumpStartCharacter::Function_LookSimple);
			LcEIC->BindAction(_Inputs.IA_Look, ETriggerEvent::Triggered, this, &AJumpStartCharacter::Function_LookSimple);
			LcEIC->BindAction(_Inputs.IA_Look, ETriggerEvent::Completed, this, &AJumpStartCharacter::Function_LookSimple);
			LcEIC->BindAction(_Inputs.IA_Look, ETriggerEvent::Canceled, this, &AJumpStartCharacter::Function_LookSimple);
		}

		// Grab Input
		LcEIC->BindAction(_Inputs.IA_Interact, ETriggerEvent::Started, this, &AJumpStartCharacter::Function_GrabPhysicsActor);
	}
}

void AJumpStartCharacter::Function_SetThirdPersonControlSettings()
{
	bUseControllerRotationYaw = false;
	M_SpringArm_Ptr->bUsePawnControlRotation = true;
	M_Camera_Ptr->bUsePawnControlRotation = false;

	if (_CanStrafeInThirdPerson) { 
		this->GetCharacterMovement()->bOrientRotationToMovement = false; }
	else { 
		this->GetCharacterMovement()->bOrientRotationToMovement = true; }

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Third Person Control Settings Applied"));
	}
}

void AJumpStartCharacter::Function_SetFirstPersonControlSettings()
{
	bUseControllerRotationYaw = true;
	M_SpringArm_Ptr->bUsePawnControlRotation = true;
	M_Camera_Ptr->bUsePawnControlRotation = false;
	this->GetCharacterMovement()->bOrientRotationToMovement = false;

	M_SpringArm_Ptr->SocketOffset = FVector(0, 0, _FirstPersonSpringArmVerticalOffset);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("First Person Control Settings Applied"));
	}
}

void AJumpStartCharacter::Function_InitializeJumpStartCharacter()
{
	
	// Set initial POV
	switch (_InitialPOV)
	{
	case E_POV::First:
		Function_SetFirstPersonControlSettings();
		M_SpringArm_Ptr->TargetArmLength = 0.0f;
		break;
	case E_POV::Third:
		Function_SetThirdPersonControlSettings();
		M_SpringArm_Ptr->TargetArmLength = _ThirdPersonSpringArmLenght;
		break;
	}
}

void AJumpStartCharacter::Function_GrabPhysicsActor()
{
	FVector LcStart = M_Camera_Ptr->GetComponentLocation();
	FVector LcEnd = LcStart + (GetControlRotation().Vector() * _POVTraceLenght);
	FQuat LcQuat = FQuat::Identity;
	FCollisionShape LcCollisionShape = FCollisionShape::MakeSphere(_GrabSphereSweepRadius);
	FCollisionQueryParams LcCollisionQueryParams;
	LcCollisionQueryParams.AddIgnoredActor(this);

	FHitResult LcHitResult;

	GetWorld()->SweepSingleByProfile(LcHitResult, LcStart, LcEnd, LcQuat, _GrabChannel.Name, LcCollisionShape, LcCollisionQueryParams);
	if (LcHitResult.bBlockingHit) {
		// Check if the hit component does simulate physics, cause it is needed to be used by the PhysicHandleComponent
		if (LcHitResult.Component->IsSimulatingPhysics()) {
			// Get the actor that owns the component that was hit and set it as ref
			_HeldedPhysicActorPtr = LcHitResult.GetActor();
			Function_CalculateGrabLocationAndRotation(_HeldedActorTargetPosition, _HeldedActorTargetRotation);
			M_PhysicHandleComponentPtr->GrabComponentAtLocationWithRotation(LcHitResult.GetComponent(), NAME_None, _HeldedActorTargetPosition, _HeldedActorTargetRotation);

			/// TO DO : Make so that while helding the actor, it changes to a "ghost" material so the player can see trought
			// apply ghost material to mesh
			// how do i acess to the mesh component? cause it may be any physic actor right? maybe a custom actor class, maybe i just put a static mesh into the world and set physics on
		}
	}
	
	DrawDebugDirectionalArrow(GetWorld(), LcStart, LcEnd, 300, FColor::Yellow, false, 3, 0, 1);
}

void AJumpStartCharacter::Function_HeldPhysicActor()
{
	Function_CalculateGrabLocationAndRotation(_HeldedActorTargetPosition, _HeldedActorTargetRotation);
	M_PhysicHandleComponentPtr->SetTargetLocationAndRotation(_HeldedActorTargetPosition, _HeldedActorTargetRotation);
}

void AJumpStartCharacter::Function_ReleaseCurrentHeldedPhysicActor(AActor*& OutActorPtr)
{
}

void AJumpStartCharacter::Function_CalculateGrabLocationAndRotation(FVector& OutPosition, FRotator& OutRotation)
{
	// calculate target rotation
	OutRotation = GetControlRotation();
	// calculate target position
	OutPosition = M_Camera_Ptr->GetComponentLocation() + (GetControlRotation().Vector() * _HeldActorPositionOffset);

	// >Debug
	DrawDebugSphere(GetWorld(), _HeldedActorTargetPosition, 10, 3, FColor::Yellow, false, -1, 1, 1);
	DrawDebugDirectionalArrow(GetWorld(), M_Camera_Ptr->GetComponentLocation(), _HeldedActorTargetPosition, 100, FColor::Yellow, false, -1, 1, 0.5f);
}

// Called every frame
void AJumpStartCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//M_JSMovementCompPtr->Function_StartClimb(); <- insted maybe should bind this to player give movement input or a manual input
	if (_HeldedPhysicActorPtr) { Function_HeldPhysicActor(); }
	Function_HeldPhysicActor();
}
void AJumpStartCharacter::Function_MoveSimple(const FInputActionValue& InValue)
{
	// Extract FVector2D from the generic FInputActionValue
	FVector2D In_Direction = InValue.Get<FVector2D>();

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

	// Set Movement Input values on JumpStart CharacterMovementComponent
	M_JSMovementCompPtr->Function_ReadOwnerMovementInput(In_Direction);
}

void AJumpStartCharacter::Function_LookSimple(const FInputActionValue& InValue)
{
	// Extract FVector2D from the generic FInputActionValue
	FVector2D LcInVector2DValue = InValue.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// Calculate the look vector based on the player input and sensitivity
		FVector2D L_LookVector(LcInVector2DValue * _LookSensitivity);

		// Apply Yaw (Look right/left)
		if (LcInVector2DValue.X != 0) { AddControllerYawInput(L_LookVector.X); }

		// Apply Pitch (Look up/down)
		if (LcInVector2DValue.Y != 0) { if (_bIsLookPitchInverted) { L_LookVector.Y = -L_LookVector.Y; } }
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


