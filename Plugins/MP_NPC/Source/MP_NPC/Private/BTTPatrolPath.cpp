// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTPatrolPath.h"

#include "AIController.h"
#include "PatrolPathComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTPatrolPath::UBTTPatrolPath()
{
	NodeName = "Patrol Path";
}

EBTNodeResult::Type UBTTPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Display, TEXT("_> EBTNodeResult::Type UBTTPatrolPath::ExecuteTask()"));
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
	UPatrolPathComponent* LcPatrolPathComponentPtr = LcPawn->FindComponentByClass<UPatrolPathComponent>();

	// step 2. checking if is valid
	if (LcPatrolPathComponentPtr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> EBTNodeResult::Type UBTTPatrolPath::ExecuteTask() - LcPatrolPathComponentPtr = nullptr"));
		return EBTNodeResult::Failed;
	}

	// step 3. extrat the path data
	FVector LcLocation = LcPatrolPathComponentPtr->Function_AskForNextPathPointLocation();

	if (LcPatrolPathComponentPtr->Function_HasAValidPath() == false) {
		UE_LOG(LogTemp, Error, TEXT("_> EBTNodeResult::Type UBTTPatrolPath::ExecuteTask() - if (LcPatrolPathComponentPtr->Function_HasAValidPath() == false)"));
		return EBTNodeResult::Failed;
	}

	// step 4. Set blackboard key value
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, LcLocation);

	if (_bDebugDraw) {
		DrawDebugSphere(GetWorld(), LcLocation, 30.0f, 6, FColor::Yellow, false, 6.0f, 1, 1);
		DrawDebugDirectionalArrow(GetWorld(), LcLocation + FVector::UpVector * 600, LcLocation, 600, FColor::Yellow, false, 6, 1, 3);
	}

	return EBTNodeResult::Succeeded;
}
