// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "MP_Stats/Public/Damageable.h"

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

	// BIND my function to OnActorHit
	// AddDynamic does not seems to appear on IntelliSense or it hides it;
	// Note that to Bind a functions his InputParameters have to be the exact same;
	OnActorHit.AddDynamic(this, &AProjectileBase::Function_OnProjectileHit);

	/*
	* for more complex similar event use
	* CollisionComp->OnComponentHit.AddDynamic
	*/
	
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
	
	// Set projectile component speeds
	//Function_CalculateAndSetProjectileSpeed(); //<-need to set this later, call on a CUSTOM initializer, maybe from weapon
	
	
}

void AProjectileBase::Function_OnProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	float LcDamage;
	if (ProjectilePDA.IsValid())
	{
		 LcDamage = ProjectilePDA.LoadSynchronous()->Damage;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[MARCO] : void AProjectileBase::Function_OnProjectileHit() : ProjectilePDA.IsValid() = false"));
		return;
	}
	
	if (OtherActor != nullptr && OtherActor->Implements<UDamageable>())
	{
		IDamageable::Execute_IFunction_ReceiveDamage(OtherActor, LcDamage);
		
		// debug feedback
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Turquoise, "ProjectileOnHit()");
		}
	}
	
	// return to pool
	this->Function_ReturnToPool();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::Function_ResetProjectile(const FTransform InLaunchTransform) // this function is called by the Pool that is called by the weapon ( so projectile hit : return to pool() > Weapon call Retrieve from pool and if so call this )
{
	// Actor state
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	bool bShouldSweep = false;
	SetActorTransform(InLaunchTransform, bShouldSweep, nullptr, ETeleportType::TeleportPhysics);

	// projectile specifics

	// HARD RESET
	_ProjectileMovementComponent->StopMovementImmediately();
	_ProjectileMovementComponent->Deactivate();
	_ProjectileMovementComponent->ResetInterpolation();

	// Reset internal state / Clear velocity explicitly
	_ProjectileMovementComponent->Velocity = FVector::ZeroVector;

	// Reactivate
	_ProjectileMovementComponent->Activate(true);

	// Relaunch (WORLD SPACE � important)
	const FVector LaunchVelocity = GetActorForwardVector() * _ProjectileMovementComponent->InitialSpeed;
	_ProjectileMovementComponent->Velocity = LaunchVelocity;
	_ProjectileMovementComponent->UpdateComponentVelocity();
}

void AProjectileBase::Function_EnterSleepState()
{
	// Actor state
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// projectile movement component
	_ProjectileMovementComponent->StopMovementImmediately();
	_ProjectileMovementComponent->Deactivate();
}

void AProjectileBase::Function_ReturnToPool()
{
	// safety check for valid pool pointer
	if (OwnerPoolPtr == nullptr) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - void AProjectileBase::Function_ReturnToPool() - OwnerPoolPtr is a nullptr!"); }
		return;
	}

	bool LcWasSuccefully;

	if (OwnerPoolPtr->GetClass()->ImplementsInterface(UIPoolable::StaticClass())) {
		IIPoolable::Execute_IFunction_ReturnToPool(OwnerPoolPtr, this, LcWasSuccefully);
		this->Function_EnterSleepState();
	}

	//if (LcWasSuccefully) {
	//	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, this->GetName() + " - void AProjectileBase::Function_ReturnToPool() was succefully!"); }
	//}
	else {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - void AProjectileBase::Function_ReturnToPool() - LcWasSuccefully = false"); }
	}
}

void AProjectileBase::Function_UpdateProjectileSpeed(const float InMultiplier)
{
	if (InMultiplier <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MARCO] : void AProjectileBase::Function_UpdateProjectileSpeed(const float InMultiplier) : InMultiplier <= 0.0f, I will set it to 1.0f !"))
		ProjectileModifiedSpeed = 1.0f;
		return;
	}
	ProjectileModifiedSpeed = InMultiplier;
}

void AProjectileBase::Function_CalculateAndSetProjectileSpeed()
{
	// safety check
	if (ProjectilePDA == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[MARCO] : void AProjectileBase::Function_CalculateAndSetProjectileSpeed() : if (ProjectilePDA == nullptr)"))
		return;
	}
	// safety check
	if (ProjectileSpeedMultiplier == 0.0f)
	{
		ProjectileModifiedSpeed = 1.0f;
	}
	// calculate
	ProjectileModifiedSpeed = ProjectilePDA->Speed * ProjectileModifiedSpeed;
	// set it
	_ProjectileMovementComponent->MaxSpeed = ProjectileModifiedSpeed;
}

void AProjectileBase::IFunction_SetOnHold_Implementation()
{
	this->Function_EnterSleepState();
}

void AProjectileBase::IFunction_ActivateActor_Implementation()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::White, this->GetName() + " - void AProjectileBase::IFunction_ActivateActor_Implementation()"); }
	//Function_ResetProjectile();
}

void AProjectileBase::IFunction_DeactivateActor_Implementation()
{
	this->Function_EnterSleepState();
	// add some logic to put this into the pool (waiting state)... for what i got so far i need to set his respective "slot" isActive parameter set to false...
	// so basicly i think i need to pass to this a reference of his slot index. or there are any other way? a way of the pool handle it without require that the object knows his slot?
	// maybe a way is to this object have a reference to the pool and call a interface or function..
}

void AProjectileBase::IFunction_ResetActorWithTransform_Implementation(FTransform InTransform)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, this->GetName() + " - void AProjectileBase::IFunction_ResetActorWithTransform_Implementation()"); }
	// this->_ResetTransform = InTransform;
	IIPoolableActor::Execute_IFunction_ActivateActor(this);
}

void AProjectileBase::IFunction_SetPointerToPoolComponent_Implementation(UActorComponent* InActorComponentPtr)
{
	if (InActorComponentPtr == nullptr)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "AProjectileBase::IFunction_SetPointerToPoolComponent_Implementation() - InActorComponentPtr = nullptr");
		UE_LOG(LogTemp, Error, TEXT("AProjectileBase::IFunction_SetPointerToPoolComponent_Implementation - InActorComponentPtr = nullptr")); return; }
	}
	OwnerPoolPtr = InActorComponentPtr;
}

void AProjectileBase::IFunction_LaunchProjectile_Implementation(const FTransform InLaunchTransform)
{
	this->Function_ResetProjectile(InLaunchTransform);
}

