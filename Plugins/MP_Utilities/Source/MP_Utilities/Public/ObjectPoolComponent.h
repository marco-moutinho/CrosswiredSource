// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "IPoolable.h"

#include "ObjectPoolComponent.generated.h"

/*
* added on 27-Jan-2026
*/
UENUM(BlueprintType)
enum class EPoolObjectState : uint8 { PsAwake, PsSleep };

USTRUCT(BlueprintType)
struct FObjectPoolSlot
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> ActorPtr;

	UPROPERTY(BlueprintReadWrite) // Deprecated - use EPoolObjectState instead
	bool IsActive;

	UPROPERTY(BlueprintReadWrite)
	EPoolObjectState PoolState;
};

/// <summary>
/// Created on 21-Jan-2026
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_UTILITIES_API UObjectPoolComponent : public UActorComponent, public IIPoolable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	float bDebugFeedbackMsgDuration = 6;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	FColor DebugFeedbackMsgColor = FColor::White;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	float bDebugErrorMsgDuration = 60;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	FColor DebugErrorMsgColor = FColor::Red;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	float DebugWarningMsgDuration = 6;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	FColor DebugWarningMsgColor = FColor::Yellow;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	float bDebugValidationMsgDuration = 6;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "[ DEBUG ]")
	FColor DebugValidationMsgColor = FColor::Green;

	UPROPERTY(BlueprintReadWrite, Category = "[ Pool Component ]")
	TArray<FObjectPoolSlot> ObjectPoolList;

	UPROPERTY(BlueprintReadWrite, Category = "[ Pool Component ]")
	int32 PoolSize;

	/*
	* By default initialize pool with at least one spawned actor
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Pool Component ]")
	virtual void Function_InitializePool();

	/*
	* This is called internaly on AProjectileWeapon::Method_SpawnProjectile();
	* Created on 21-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Pool Component ]")
	virtual void Function_AddToPool(AActor* InActorPtr, bool bInIsActive);

	/*
	* "Activates" a actor from the pool if possible; If not then spawn a new actor and add it to the pool;
	* Created on 21-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Pool Component ]")
	virtual void Function_RetrieveFromPool(bool& OutSuccessfullyRetrived);


	/*
	* "Activates" a actor from the pool if possible; If not then spawn a new actor and add it to the pool;
	* This is also called internaly on AProjectileWeapon::Function_Shoot();
	* Calls IFunction_ResetActorWithTransform internally;
	* Created on 21-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Pool Component ]")
	virtual void Function_RetrieveFromPoolWithTransform(FTransform InTransform, bool& OutSuccessfullyRetrived);

	/*
	* On my MP_Weapons plugin this function is called by the projectile
	*/
	// Inherited via IIPoolable
	void IFunction_ReturnToPool_Implementation(AActor* InActorPtr, bool& bOutSucceded) override;
};
