// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolComponent.h"
#include "IPoolableActor.h"

// Sets default values for this component's properties
UPoolComponent::UPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::UPoolComponent()"));
}


// Called when the game starts
void UPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::BeginPlay()"));
}


// Called every frame
void UPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPoolComponent::Function_InitializePool(const TSubclassOf<AActor> InActorClass, const int32 InPoolSize, const EPoolState InInitialPoolState)
{
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::Function_InitializePool()"));
	if (InActorClass == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + "::Function_InitializePool() - InActorClass = nullptr"); } return; }
	
	// Set CurrentActorSubClass - it will be needed later to spawn new actor if none could be acquire
	CurrentActorSubClass = InActorClass;
	
	// InPoolSize is a input parameter cause I have read in the doc about .Empty this : 
	// "This method potentially leaves space allocated for an expected number of elements about to be added." and "ExpectedNumElements The number of elements about to be added to the set."
	// So basicly clear pool
	PoolMap.Empty(InPoolSize); 

	// populate pool
	for (int32 i = 0; i < InPoolSize; i++) { this->Function_AddToPool( Function_SpawnActorOfClass(InActorClass, InInitialPoolState), InInitialPoolState); }
}

AActor* UPoolComponent::Function_SpawnActorOfClass(const TSubclassOf<AActor> InActorClass, const EPoolState InSpawnPoolState)
{
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::Function_SpawnActorOfClass()"));
	// Safety check : for valid class
	if (InActorClass == nullptr) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_SpawnActorOfClass() - InActor class in invalid"); } // this trigger when i shoot
		return nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::Function_SpawnActorOfClass() : class :%s"), *InActorClass->GetName());

	// Create SpawnParams
	FActorSpawnParameters LcActorSpawnParams;
	LcActorSpawnParams.Owner = (this->GetOwner());
	LcActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LcActorSpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale; // spawn with normal actor scale
	// LcActorSpawnParams.Instigator = LcActorSpawnParams.Owner->GetOwner(); <- can i set the instigator after spawn on for exemple in the weapon class that has a ref of his character ownerr'

	// start Spawn actor
	AActor* LcSpawnedActorPtr = GetWorld()->SpawnActorDeferred<AActor>(InActorClass,  FTransform::Identity, this->GetOwner(), nullptr, LcActorSpawnParams.SpawnCollisionHandlingOverride, LcActorSpawnParams.TransformScaleMethod);

	// call interface to poolableactor setup 
	// ...then set the projectile spefifics via weapon, cause this func returns a actor* that can be used access it
	if (LcSpawnedActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()))
	{
		// set actor poolreference to this/self
		IIPoolableActor::Execute_IFunction_SetPointerToPoolComponent(LcSpawnedActorPtr, this);

		switch (InSpawnPoolState)
		{
		case EPoolState::AwakeSt:
			break;

		case EPoolState::SleepSt:
			IIPoolableActor::Execute_IFunction_SetOnHold(LcSpawnedActorPtr); /// IS IT??? how should the actor spawn? maybe better to give this function (UPoolComponent::Function_SpawnActorOfClass) a input parameter to decide
			break;
		}
		
	}
	else {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_SpawnActorOfClass() - LcSpawnedActorPtr class does not implement UIPoolable"); }
		UE_LOG(LogTemp, Error, TEXT("UPoolComponent::Function_SpawnActorOfClass() - LcSpawnedActorPtr->GetClass()->ImplementsInterface()) = FALSE)"));
		return nullptr; }
	
	if (LcSpawnedActorPtr){
		LcSpawnedActorPtr->FinishSpawning(FTransform::Identity);
		return LcSpawnedActorPtr;
	}
	else{ if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_SpawnActorOfClass() - LcSpawnedActorPtr is invalid"); } return nullptr; }
}

void UPoolComponent::Function_AddToPool(AActor* InActorPtr, const EPoolState InPoolState)
{
	UE_LOG(LogTemp, Log, TEXT("UPoolComponent::Function_AddToPool()"));
	// Safety check
	if (InActorPtr == nullptr) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_AddToPool(AActor* InActorPtr) : InActorPtr is a nullptr"); } // this trigger when i shoot
		return;
	}

	// safety check
	if (PoolMap.Contains(InActorPtr)) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_AddToPool() - PooMap already contains InActorPtr"); } return; }

	// add it
	PoolMap.Add(InActorPtr, InPoolState);
}

AActor* UPoolComponent::Function_AcquireActorPtr()
{
	// find a valid "slot" - a actor* key that has a value of sleep
	for (auto& slot : PoolMap) {

		// if value is sleep ( meaning that can be acquire ) proced
		if (slot.Value == EPoolState::SleepSt)
		{
			// Elem.Key.Get() is used because TObjectPtr is a smart pointer.
			AActor* LcActorPtr = slot.Key.Get();

			// safety check
			if (LcActorPtr == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - Function_AcquireActorPtr() -  CRITICAL ERROR"); return nullptr; } }

			// Set actor state "pool slot" key-value = Awake right?
			slot.Value = EPoolState::AwakeSt;
			return LcActorPtr;
		}
	} /// loop end

	// REVIEW this vv
	// if none actor couldn't be acquire, spawn a new one and add it to the pool
	AActor* LcSpawnedActorPtr = Function_SpawnActorOfClass(CurrentActorSubClass, EPoolState::SleepSt);
	this->Function_AddToPool(LcSpawnedActorPtr, EPoolState::SleepSt); // it enters on sleep state cause the Class that requires it will call it from the pool next and set his internals

	// DEBUG feedback msg
	UE_LOG(LogTemp, Warning, TEXT("UPoolComponent::Function_AcquireActorPtr() : none actor couldn't be acquire, so SPAWNED a new and ADDED to the pool"));

	// return
	return LcSpawnedActorPtr;
}

void UPoolComponent::Function_ReturnToPool(AActor* InActorPtr)
{
	// safety checks
	if (InActorPtr == nullptr) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_ReturnToPool(AActor* InActorPtr) - InActorPtr is a nullptr"); return; } }
	if (PoolMap.Contains(InActorPtr) == false) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - ::Function_ReturnToPool(AActor* InActorPtr) - PoolMap does NOT contain InActorPtr as a value"); return; } }

	// Find this actor on pool "map" and set his correspondent value "pool state" to Sleep
	EPoolState* LcValuePtr = PoolMap.Find(InActorPtr);
	*LcValuePtr = EPoolState::SleepSt; // using *var means “Go to the memory location this pointer is pointing to.” - This is called dereferencing.
	// dont forget to Desativate the actor it self
	//... here or on the actor it self?
}

void UPoolComponent::IFunction_ReturnToPool_Implementation(AActor* InActorPtr, bool& bOutSucceded)
{
	// Safety check
	if (InActorPtr == nullptr) {if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " ::IFunction_ReturnToPool_Implementation(...) - InActorPtr = nullptr"); bOutSucceded = false; return; }}
	this->Function_ReturnToPool(InActorPtr);
}