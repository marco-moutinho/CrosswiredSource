// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable tick
	SetActorTickEnabled(false); // Disabled by default, can be enabled later on

	// Initialize Weapon Mesh Component
	Method_InitializeWeaponVisuals();

	//Debug message to indicate constructor call
	if(GEngine && bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, GetName() + " | AWeaponBase::Constructor() | Base");
	}

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if(GEngine && bDebugMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, GetName() + " | AWeaponBase::BeginPlay()");
	}  
}

void AWeaponBase::Function_InitializeExternalPointers(APawn* InOwningPawn)
{
	if (InOwningPawn == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - AWeaponBase::Function_InitializeExternalPointers - InOwningPawn is nullptr"); } }
	OwningPawnPtr = InOwningPawn;
}

void AWeaponBase::Method_InitializeWeaponVisuals()
{
	// Create the Static Mesh Component for the Weapon
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	// Attach the Weapon Mesh to the Root Component
	RootComponent = WeaponMeshComp;
}

void AWeaponBase::Function_SetCameraComponentPointer(UCameraComponent* InCameraComponentPtr)
{
	PlayerCameraCompPtr = InCameraComponentPtr;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA)
{
	// Safety check
	if (!WeaponDefinitionPDA)
	{
		// [Warning] Debug message if WeaponDefinitionPDA is null
		if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - AWeaponBase::Function_InitializeFromDefinition - WeaponDefinitionPDA is null"); }
		return;
	}
}

void AWeaponBase::Function_ExecuteWeaponAction()
{
	/*
	* To be overridden in child classes
	*/
}

