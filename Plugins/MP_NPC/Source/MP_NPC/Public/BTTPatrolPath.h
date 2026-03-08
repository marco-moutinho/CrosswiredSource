// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTPatrolPath.generated.h"
// created on 08-Mar-2026
/**
 * 
 */
UCLASS()
class MP_NPC_API UBTTPatrolPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTPatrolPath();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "[ MP_NPC ]", meta = (DisplayName = "DebugDraw Enabled?"))
	bool _bDebugDraw = false;
};
