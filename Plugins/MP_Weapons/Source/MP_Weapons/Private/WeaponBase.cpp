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

	if(GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, GetName() + " | AWeaponBase::Tick()"); }
}

void AWeaponBase::Function_InitializeFromDefinition(UWeaponDefinitionPDA* WeaponDefinitionPDA)
{
}

void AWeaponBase::Function_StartUse()
{
	// Debug message to indicate method call
	if (GEngine && bDebugMode) { GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, GetName() + " | AWeaponBase::Method_StartUse()"); }
}

void AWeaponBase::Function_StopUse()
{
}

void AWeaponBase::Method_Using()
{
}

