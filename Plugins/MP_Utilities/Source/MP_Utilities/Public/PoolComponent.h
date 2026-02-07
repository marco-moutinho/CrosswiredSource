// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "IPoolable.h"

#include "PoolComponent.generated.h"

// Created on 05-Fev-2026
UENUM(BlueprintType)
enum class EPoolState : uint8 { AwakeSt, SleepSt };

/*
* This class is responsible to : Store Actor pointers; Return actor pointers; If a slot is available "awake" the actor, if none slot is available spawn a new one; Receive calls from outside to "return" actors to the pool (sleep state)
*  [ N O T E ]
* (A) - currently this class is meant to work with only one actor class type at a time ("mono actor class" pool) cause this component was meant initially to make a projectile pool component for weapons, so on the future I'l problably refactor this to work with multiple actor classes at a time;
* (B) - this is a "passive component" it only receives calls and it's supposed to be managed by a owner actor;
* (C) - ...
* Created on 03-Fev-2026 ( A complete refactor of intial pool component ) | Last Change : 07-Fev-2026
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_UTILITIES_API UPoolComponent : public UActorComponent,  public IIPoolable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/*Map summary
	* value true : is a sleep/can be acquired
	* value false : is active/can not be acquired
	* // Added on 03-Fev-2026
	*/
	UPROPERTY()
	TMap<TObjectPtr<AActor>, EPoolState> PoolMap; 

	// Added on 03-Fev-2026
	UPROPERTY()
	int32 InitialPoolSize;

	/*
	* This is needed to spawn new actors, mainly in case none of actors on pool is available to be acquired/retrieved
	* Added on 04-Fev-2026
	*/
	UPROPERTY()
	TSubclassOf<AActor> CurrentActorSubClass;

	/*
	* This functions is meant to be called by the owner
	*  Added on 03-Fev-2026
	*/
	UFUNCTION()
	void Function_InitializePool(const TSubclassOf<AActor> InActorClass, const int32 InPoolSize, const EPoolState InInitialPoolState);

protected:
	/*
	* This function is suposed to be called internally only, currently called by "Function_InitializePool" and on "Function_AcquireActorPtr"
	* Added on 03-Fev-2026
	*/
	UFUNCTION()
	AActor* Function_SpawnActorOfClass(const TSubclassOf<AActor> InActorClass, const EPoolState InSpawnPoolState);

	/*
	* This function is suposed to be called internally only, currently called by "Function_InitializePool" and on "Function_AcquireActorPtr"
	* bInState = true : means that the actor is available to be retrieved meaning is in "sleep state"
	* bInState = false : means that the actor is unavailable to be retrieved, meaning that is active / "awake state"
	* Added on 03-Fev-2026 / Last change : 04-Fev-2026
	*/
	UFUNCTION()
	void Function_AddToPool(AActor* InActorPtr, const EPoolState InPoolState);

public:
	/*
	* It returns a actor from the pool by searching one slot available ( "sleep" state ), if none was found then it will spawn (SpawnActorDeferred) a new one and automaticly added to the pool, the Actor will spawn on "Sleep State" so that the class that calls for a new one decides what happens to it;
	*  Added on 03-Fev-2026
	*/
	UFUNCTION()
	AActor* Function_AcquireActorPtr();
protected:
	/*
	*  Added on 03-Fev-2026 / Last change : 04-Fev-2026
	*/
	UFUNCTION()
	void Function_ReturnToPool(AActor* InActorPtr);

public:
	virtual void IFunction_ReturnToPool_Implementation(AActor* InActorPtr, bool& bOutSucceded) override; // Implementation of IPoolable
		
};
