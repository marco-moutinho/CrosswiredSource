// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AiControllerAccessiable.h"
#include "BehaviorTree/BehaviorTree.h"

#include "AIController.h"
#include "NPCController.generated.h"

class UNavigationSystemV1;

/**
 * started to work on 07-Mar-2026
 */
UCLASS()
class MP_NPC_API ANPCController : public AAIController, public IAiControllerAccessiable
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// Jumpstart Propertys
	//...
	UPROPERTY(EditDefaultsOnly, Category = "[ Jumpstart NPC ]")
	bool _bAutoInit = true;

	UPROPERTY(EditDefaultsOnly, Category = "[ Jumpstart NPC ]")
	UBehaviorTree* _BehaviorTreePtr;

	//RTO...
	//...
	UPROPERTY()
	UNavigationSystemV1* _NavSystemPtr;

	// added on 07-Mar-2026
	UFUNCTION()
	void Function_GetNavigationSystem();

	// via "IAiControllerAccessiable" - added on 07-Mar-2026
	virtual void IFunction_GetNavigationSystemPtr_Implementation(UNavigationSystemV1*& OutPtr) override;
};
