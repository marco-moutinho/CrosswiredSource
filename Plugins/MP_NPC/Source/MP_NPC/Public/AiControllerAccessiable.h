// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AiControllerAccessiable.generated.h"

class UNavigationSystemV1;

// Created on 07-Mar-2026

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAiControllerAccessiable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MP_NPC_API IAiControllerAccessiable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// added on 07-Mar-2026
	UFUNCTION(BlueprintNativeEvent)
	void IFunction_GetNavigationSystemPtr(UNavigationSystemV1*& OutPtr);
};
