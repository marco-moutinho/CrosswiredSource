// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTPatrolPath.h"

#include "AIController.h"
#include "PatrolPathComponent.h"

UBTTPatrolPath::UBTTPatrolPath()
{
	NodeName = "Patrol Path";
}

EBTNodeResult::Type UBTTPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AiController
	AAIController* LcAiControllerPtr = OwnerComp.GetAIOwner();

	// Safety check
	if (LcAiControllerPtr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AAIController* LcAiControllerPtr = nullptr"));
		return EBTNodeResult::Failed;
	}

	// Get controlled/posseced pawn
	APawn* LcPawn = LcAiControllerPtr->GetPawn();

	// safety check
	if (LcPawn == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("APawn* LcPawn = nullptr"));
		return EBTNodeResult::Failed;
	}

	// Access to PatrolPath...
	// by checking if pawn/NPC has the PatrolPathComponent - this component must return the path data
	
	// setp 1. try get component directly
	const UPatrolPathComponent* LcPatrolPathComponentPtr = LcPawn->FindComponentByClass<UPatrolPathComponent>();

	// step 2. checking if is valid
	if (LcPatrolPathComponentPtr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("const UPatrolPathComponent* LcPatrolPathComponentPtr = nullptr"));
		return EBTNodeResult::Failed;
	}

	// step 3. extrat the path data

	return EBTNodeResult::Failed;
}
