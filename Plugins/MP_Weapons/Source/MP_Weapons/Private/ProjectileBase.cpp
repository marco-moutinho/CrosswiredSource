// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Method_InitializeMeshComponent();
	Method_InitializeProjectileComponent();

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::Method_InitializeMeshComponent()
{
	// first: create the static mesh component and store it in the variable
	_ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Component"));

	// second: the static mesh component as the root component
	RootComponent = _ProjectileMeshComponent;
}

void AProjectileBase::Method_InitializeProjectileComponent()
{
	// firstly, create the projectile movement component and store it in the variable
	_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	
	// set the projectile movement component properties
	_ProjectileMovementComponent->UpdatedComponent = RootComponent;
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugDirectionalArrow(
	//	GetWorld(),
	//	GetActorLocation(), // Start
	//	GetActorLocation() + (GetActorForwardVector() * 50.0f), // End
	//	100.0f, // ArrowSize
	//	FColor::Blue,
	//	false, // bPersistentLines
	//	-1, // LifeTime
	//	0, // DepthPriority
	//	3.0f // Thickness
	//);

	//float SRadius = 5;
	//int SSegments = 6;
	//float SLifetime = -1;
	//uint8 SDepth = 0;
	//float SThickness = 1;

	//DrawDebugSphere(GetWorld(), this->GetActorLocation(), SRadius, SSegments, FColor::Turquoise, false, SLifetime, SDepth, SThickness);

}

