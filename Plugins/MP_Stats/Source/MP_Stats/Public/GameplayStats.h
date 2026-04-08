// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayTagContainer.h"


#include "GameplayStats.generated.h"

// DEprecated : but maybe usefull in the future if someone request a more robust function that automatic handles what math operation it wants to apply tge DELTA
UENUM(BlueprintType)
enum class EStateChangeMode : uint8
{
	Increase,
	Decrease,
	Multiply,
	Divide,
	Absolute,
};

UENUM(BlueprintType)
enum class EStatParamProperty : uint8
{
	Current,
	Max,
	Min,
	RegainRate,
	DrainRate,
};

// created on 14-mar-2026
USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	EStateChangeMode ModifierType = EStateChangeMode::Increase;
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float ModifierValue = 0.0f;
	
};

// created on 14-Mar-2026
USTRUCT(BlueprintType)
struct FStatParams
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float Current = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float Max = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float Min = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float RegainRate = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float DrainRate = 0.0f;
	
	float GetCurrentNormalized() const
	{
		return Current / Max;
	}
	
	UPROPERTY()
	TArray<FStatModifier> Modifiers = TArray<FStatModifier>();
};

USTRUCT(BlueprintType)
struct FElementalStatParams
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	float Affinity;
};


// created on 14-Mar-2026
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_STATS_API UGameplayStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	protected:
	UPROPERTY(EditDefaultsOnly, Category = "[ GameplayStats ]")
	TMap<FGameplayTag, FStatParams> Stats;
	
	// added 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GameplayStats Functions ]" )
	virtual void Function_IncreaseTargetPropertyOfSelectedStat(const FGameplayTag& InStatTag, const EStatParamProperty InProperty, const float InDelta);
	
	// added 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GameplayStats Functions ]")
	virtual void Function_DecreaseTargetPropertyOfSelectedStat(const FGameplayTag InStatTag, const EStatParamProperty InProperty, const float InDelta);
	
	// added 14-Mar-2026
	//UFUNCTION(BlueprintCallable, Category = "[ GameplayStats Functions ]")
	//virtual void Function_MultiplyCurrentOfStat(const FGameplayTag InStatTag, const float InDelta);
	
	// wrote 14-Mar-2026
	//UFUNCTION(BlueprintCallable, Category = "[ GameplayStats Functions ]")
	//virtual void Function_OnStatHasReachedMinimum();
	// exemple kill character, destroy actor, notify player, idk...
};
