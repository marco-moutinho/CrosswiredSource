// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTMoveToRandomLocation.generated.h"
// created on 07-Mar-2026
/**
 * 
 */
UCLASS()
class MP_NPC_API UBTTMoveToRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTMoveToRandomLocation(); // constructer
	
protected:
	UPROPERTY(EditAnywhere, Category = "[ MP_NPC ]", meta = (DisplayName = "Go to a reachable location?"))
	bool _bReachableLocation = true;

	UPROPERTY(EditAnywhere, Category = "[ MP_NPC ]", meta = (DisplayName = "Radius of search"))
	float _RadiusOfSearch = 2000.0f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
