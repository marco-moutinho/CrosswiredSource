// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
// created on 14-Mar-2025
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MP_STATS_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, Category = "[ MP_Stats ]|IDamageable")
	void IFunction_ReceiveDamage(const float InDelta);
	
	UFUNCTION(BlueprintNativeEvent, Category = "[ MP_Stats ]|IDamageable")
	void IFunction_OnHealthHasReachedZero();
	
};
