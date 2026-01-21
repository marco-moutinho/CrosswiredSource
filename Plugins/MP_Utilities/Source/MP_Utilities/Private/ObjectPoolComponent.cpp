// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"
#include "IPoolableActor.h"

// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObjectPoolComponent::Function_InitializePool()
{
}

void UObjectPoolComponent::Function_AddToPool(AActor* InActorPtr, bool bInIsActive)
{
	// Safety check : if this actor is already on the pool:
	for (FObjectPoolSlot& poolSlot: ObjectPoolList)
	{
		if (InActorPtr == poolSlot.ActorPtr){
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - void UObjectPoolComponent::Function_AddToPool - ActorPtr already exist on this poolm !"); }
			return;
		}
	}
	
	// create a new ObjectPoolSlot struct
	FObjectPoolSlot LcNewPoolObject;
	LcNewPoolObject.ActorPtr = InActorPtr;
	if(bInIsActive){ LcNewPoolObject.IsActive = true; }
	else { LcNewPoolObject.IsActive = false; }
	
	// added to the pool
	ObjectPoolList.Add(LcNewPoolObject);

	// DEBUG FEEDBACK
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, "UObjectPoolComponent::Function_AddToPool - A new Pool Slot was ADDED !"); }
}

void UObjectPoolComponent::Function_RetrieveFromPool(bool& OutSuccessfullyRetrived)
{
	// if the pool is empty than spawn a new actor
	if (ObjectPoolList.IsEmpty())
	{
		OutSuccessfullyRetrived = false;
		return;
		// somewhere: create/spawn the actor and then add to this pool
	}
	// debug msg
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, this->GetName() + " - UObjectPoolComponent::Function_RetrieveFromPool"); }

	/*
	*  NOTE on "&"
	* Without & (value copy)
	*      is a copy of each element in ObjectPoolList.
	*	   Any changes you make (Slot.IsActive = true) only affect the temporary copy, not the real element in the array.
	*	   After the loop iteration ends, the copy is discarded - the pool is unchanged.
	* With & (reference)
	*      Slot is now a reference to the actual element in ObjectPoolList.
	*	   Changes you make (Slot.IsActive = true) persist in the array.
	*      No copies are made - slightly better performance too.
	* Optional: "const &"
	*      If you don’t intend to modify the element;
	*	   Guarantees you won’t accidentally change anything.
	*	   Still avoids making a copy.
	*/

	// find the a retrievable object from pool
	for (FObjectPoolSlot& slot : ObjectPoolList)
	{
		if (slot.ActorPtr && slot.IsActive == false)
		{
			if (slot.ActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()))
			{
				IIPoolableActor* InterfaceImplementationPtr = Cast<IIPoolableActor>(slot.ActorPtr);
				InterfaceImplementationPtr->IFunction_ActivateActor();
				
				slot.IsActive = true;
				
				OutSuccessfullyRetrived = true;
			}
			
			OutSuccessfullyRetrived = false;
		}
	}

}

void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform(FTransform InTransform, bool& OutSuccessfullyRetrived)
{
	// if the pool is empty than spawn a new actor
	if (ObjectPoolList.IsEmpty())
	{
		OutSuccessfullyRetrived = false;
		return;
		// somewhere: create/spawn the actor and then add to this pool
	}

	int LcSlotCount = 0;

	// find the a retrievable object from pool
	for (FObjectPoolSlot& slot : ObjectPoolList)
	{
		LcSlotCount++;

		// this means that the Slot is valid;
		if (slot.ActorPtr && slot.IsActive == false)
		{
			// this just check if actor on slot has the needed interface
			if (slot.ActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()))
			{
				IIPoolableActor* InterfaceImplementationPtr = Cast<IIPoolableActor>(slot.ActorPtr);
				InterfaceImplementationPtr->IFunction_ResetActorWithTransform(InTransform);

				slot.IsActive = true;

				OutSuccessfullyRetrived = true;

				if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - Retrieve was a SUCCESS !!!"); }

				break;
			}
			
			// if it tuns till this line of code it means that there are no valid slots on pool, meaning that it is neeeded to trully spawn a new actor
			// I do that outside of this component, like for exemple on the ProjectileWeapon class
			// thats why this function has a bool output parameter, to give feedback
		}
		// this means that it loops for all and none was retrievable
		if (LcSlotCount == ObjectPoolList.Num())
		{
			OutSuccessfullyRetrived = false;
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - None of the slots on pull was RETRIEVABLE..."); }
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Purple, FString::Printf(TEXT("%s - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - PoolSize = %d"), *GetName(), LcSlotCount)); }
			return;
		}
		
	}
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, this->GetName() + " - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - This means that the for loop runs completly and did nothing"); }

	/*
	*  NOTE on "&"
	* Without & (value copy)
	*      is a copy of each element in ObjectPoolList.
	*	   Any changes you make (Slot.IsActive = true) only affect the temporary copy, not the real element in the array.
	*	   After the loop iteration ends, the copy is discarded - the pool is unchanged.
	* With & (reference)
	*      Slot is now a reference to the actual element in ObjectPoolList.
	*	   Changes you make (Slot.IsActive = true) persist in the array.
	*      No copies are made - slightly better performance too.
	* Optional: "const &"
	*      If you don’t intend to modify the element;
	*	   Guarantees you won’t accidentally change anything.
	*	   Still avoids making a copy.
	*/

}

