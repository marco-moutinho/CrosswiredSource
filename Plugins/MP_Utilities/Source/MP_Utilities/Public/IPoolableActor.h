// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPoolableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * Created on 21-Jan-2026
 */
class MP_UTILITIES_API IIPoolableActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// added on 21-Jan-2026
	virtual void IFunction_ActivateActor() = 0; 
	/*
	* it's supposed to call IFunction_ActivateActor() internally / on his cpp implementation
	* // added on 21-Jan-2026
	*/
	virtual void IFunction_ResetActorWithTransform(const FTransform InTransform) = 0; 
	// added on 21-Jan-2026
	virtual void IFunction_DeactivateActor() = 0; 

};
