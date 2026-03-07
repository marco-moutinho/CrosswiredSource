// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTMoveToRandomLocation.h"

#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AiControllerAccessiable.h"

UBTTMoveToRandomLocation::UBTTMoveToRandomLocation()
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTMoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Display, TEXT("UBTTMoveToRandomLocation::ExecuteTask"));

	// Refactor the system so the AiController / Pawn / UNavigationSystemV1* can be set once insted per this node executionCall...
	// maybe be variables on Blackboard, or just stored on AiControlled returned by a interface on it or something...
	// the way i am making seems expensive...
	AAIController* LcAiControllerPtr = OwnerComp.GetAIOwner();
	APawn* LcPawn = LcAiControllerPtr->GetPawn();

	// Safety Check : for AiController
	if ( LcAiControllerPtr == nullptr ) {
		UE_LOG(LogTemp, Display, TEXT("Lc_AiControllerPtr is nullptr"));
		return EBTNodeResult::Failed;
	}

	// Safety check : for interface implementation
	if ( LcAiControllerPtr->GetClass()->ImplementsInterface(UAiControllerAccessiable::StaticClass()) == false ) {
		// Debug log
		UE_LOG(LogTemp, Fatal, TEXT("LcAiControllerPtr Class does not implements UAiControllerAccessiable::StaticClass"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* LcNavSystemPtr;
	IAiControllerAccessiable::Execute_IFunction_GetNavigationSystemPtr(LcAiControllerPtr, LcNavSystemPtr);
	
	if (LcNavSystemPtr == nullptr) {
		UE_LOG(LogTemp, Fatal, TEXT("LcNavSystemPtr = nullptr"));
		return EBTNodeResult::Failed;
	}
	FNavLocation LcNavLocation;
	if (_bReachableLocation == true && _RadiusOfSearch != 0.0f) {
		LcNavSystemPtr->GetRandomReachablePointInRadius(LcPawn->GetActorLocation(), _RadiusOfSearch, LcNavLocation);
	}
	// dont need to be in reach but designer has assign a valid radius value
	else if (_RadiusOfSearch != 0.0f){
		LcNavSystemPtr->GetRandomPointInNavigableRadius(LcPawn->GetActorLocation(), _RadiusOfSearch, LcNavLocation);
	}
	// just in case of designer has radius = 0
	else {
		LcNavSystemPtr->GetRandomPoint(LcNavLocation);
	}

	FVector LcLocation = LcNavLocation.Location;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, LcLocation);

	DrawDebugSphere(GetWorld(), LcLocation, 50, 6, FColor::Green, false, 10, 1, 1);

	return EBTNodeResult::Succeeded;
}
