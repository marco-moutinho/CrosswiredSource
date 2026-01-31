// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolable.generated.h"
/*
* added on 26-Jan-2026
*/
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UIPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MP_UTILITIES_API IIPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent) // added on 26-Jan-2026
	void IFunction_ReturnToPool(AActor* InActorPtr, bool& bOutSucceded);
};
