// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolSlot
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> ActorPtr;

	UPROPERTY(BlueprintReadWrite)
	bool IsActive;
};
/// <summary>
/// Created on 21-Jan-2026
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_UTILITIES_API UObjectPoolComponent : public UActorComponent
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
	* This is also called internaly on AProjectileWeapon::Function_Shoot();
	* Created on 21-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Pool Component ]")
	virtual void Function_RetrieveFromPool(bool& OutSuccessfullyRetrived);
};
