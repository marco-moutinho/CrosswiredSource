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
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, this->GetName() + " - void UObjectPoolComponent::Function_AddToPool - ActorPtr already exist on this pool !"); }
			return;
		}
		else { continue; }
	}
	
	// create a new ObjectPoolSlot struct
	FObjectPoolSlot LcNewPoolObject;
	LcNewPoolObject.ActorPtr = InActorPtr;
	if(bInIsActive)
	{ 
		LcNewPoolObject.PoolState = EPoolObjectState::PsAwake;
	}
	else { LcNewPoolObject.PoolState = EPoolObjectState::PsSleep; }
	
	// added to the pool
	ObjectPoolList.Add(LcNewPoolObject);

	// DEBUG FEEDBACK
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "UObjectPoolComponent::Function_AddToPool - A new Pool Slot was ADDED !"); }
}

void UObjectPoolComponent::Function_RetrieveFromPool(bool& OutSuccessfullyRetrived) // Deprecated ( i will not use it anymore and use the Function_RetrieveFromPoolWithTransform) but i will leave it if i want to re implement it later
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
		if (slot.ActorPtr && slot.PoolState == EPoolObjectState::PsSleep)
		{
			if (slot.ActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()))
			{
				IIPoolableActor::Execute_IFunction_ActivateActor(slot.ActorPtr);
				slot.PoolState = EPoolObjectState::PsAwake;
				OutSuccessfullyRetrived = true;
				return;
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
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, DebugWarningMsgDuration, DebugWarningMsgColor, this->GetName() + " - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform() - ObjectPoolList.IsEmpty!"); }
		OutSuccessfullyRetrived = false;
		return;
		// somewhere: create/spawn the actor and then add to this pool
	}

	int LcSlotCount = 0;

	// find the a retrievable object from pool
	for (FObjectPoolSlot& slot : ObjectPoolList)
	{
		/// Loop Body Begining
		LcSlotCount++;

		// this means that the Slot is valid;
		if (slot.ActorPtr && slot.PoolState == EPoolObjectState::PsSleep)
		{
			// this just a safety check if actor on slot has the needed interface
			if (slot.ActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()))
			{
				// call the interface implementation on actor*
				IIPoolableActor::Execute_IFunction_ResetActorWithTransform(slot.ActorPtr, InTransform);

				// also set his slot to active / being used
				slot.PoolState = EPoolObjectState::PsAwake;

				// return output parameter
				OutSuccessfullyRetrived = true;

				// feedback msg
				if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, bDebugValidationMsgDuration, DebugValidationMsgColor, this->GetName() + " - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - Retrieve was a SUCCESS !!!"); }

				// End loop cause it found a object of pool to activate
				return; 
			}
			//else
			//{
			//	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Silver, this->GetName() + " ::Function_RetrieveFromPoolWithTransform - Invalid slot..."); }
			//	continue;
			//}
		}
		// this means that it loops for all and none was retrievable
		/*if (LcSlotCount == ObjectPoolList.Num())
		{
			OutSuccessfullyRetrived = false;
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - None of the slots on pull was RETRIEVABLE..."); }
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Purple, FString::Printf(TEXT("%s - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform - PoolSize = %d"), *GetName(), LcSlotCount)); }
			return;
		}*/
		else
		{
			continue;
		}
		/// Loop Body End
	}
	/// the code should only run up to this line if did not find any retriavable slot, cause if it did the function should had stop running via "return"
	OutSuccessfullyRetrived = false;
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, DebugWarningMsgDuration, DebugWarningMsgColor, this->GetName() + " - void UObjectPoolComponent::Function_RetrieveFromPoolWithTransform -  did not find any retriavable slot !"); }

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

void UObjectPoolComponent::IFunction_ReturnToPool_Implementation(AActor* InActorPtr, bool& bOutSucceded)
{
	// Debug msg to know if my projectiles are being called
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, bDebugFeedbackMsgDuration, DebugFeedbackMsgColor, this->GetName() + "UObjectPoolComponent::IFunction_ReturnToPool( " + InActorPtr->GetName() + " )"); }

	// pre set t failure, then in this function body set it to success if so
	bOutSucceded = false;
	
	// safety check : for a valid input AActor*
	if (InActorPtr == nullptr) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, bDebugErrorMsgDuration, DebugErrorMsgColor, this->GetName() + " - void UObjectPoolComponent::IFunction_ReturnToPool() - InActorPtr is a nullptr !"); }
		return;
	}
	// safety check : in case of InActorPtr does not implements the correct interface
	if (InActorPtr->GetClass()->ImplementsInterface(UIPoolableActor::StaticClass()) == false) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, bDebugErrorMsgDuration, DebugErrorMsgColor, this->GetName() + " - void UObjectPoolComponent::IFunction_ReturnToPool() - InActorPtr does not implements IPoolableActor interface !"); }
		return;
	}

	// find this actor on the pool ( is this the best way of findn a specific ptr on a array?
	// NEVER forget about the & to interact as a ref and not a copy!
	for (FObjectPoolSlot& LcCurrentSlot : ObjectPoolList) {
		
		// If InActorPtr is equal to the CurrentSlot.ActorPtr means that the object exist on the pool
		if (InActorPtr == LcCurrentSlot.ActorPtr)
		{
			// just check if he his Active, cause if it is not I did something very strange or something very strange that I did not predict has happen
			if (LcCurrentSlot.PoolState == EPoolObjectState::PsSleep) {
				if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, bDebugErrorMsgDuration, DebugErrorMsgColor, this->GetName() + " - void UObjectPoolComponent::IFunction_ReturnToPool() - LcCurrentSlot.ActorPtr is valid BUT LcCurrentSlot.IsActive is false !!!"); }
				return;
			}

			// Call interface on Actor to "enter on Sleep State"
			IIPoolableActor::Execute_IFunction_DeactivateActor(InActorPtr);

			// set this slot state to Sleep
			LcCurrentSlot.PoolState = EPoolObjectState::PsSleep;

			bOutSucceded = true;
		}
	}
	/// NOTES :
	/// should i make a for loop on pool to find the correspondent slot? and the interact based on that
	/// Set the object to "Sleep State" - NOTE : I probably create a enum or a var or a function with this name/concept instead of activate/deactivate
}

