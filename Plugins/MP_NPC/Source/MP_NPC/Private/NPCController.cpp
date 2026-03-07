// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"

#include "NavigationSystem.h"
#include "NavigationPath.h"

void ANPCController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Hello World - NPCController"));
	if (_bAutoInit) {
		//RunBehaviorTree()
		Function_GetNavigationSystem();

		if (_BehaviorTreePtr != nullptr) {
			RunBehaviorTree(_BehaviorTreePtr);
		}
	}
}

void ANPCController::Function_GetNavigationSystem() // WHEN should I call this? on BeginPlay? well it has to be before run the behavior tree thats for sure
{
	_NavSystemPtr = UNavigationSystemV1::GetCurrent(GetWorld());
}

void ANPCController::IFunction_GetNavigationSystemPtr_Implementation(UNavigationSystemV1*& OutPtr)
{
	OutPtr = _NavSystemPtr;
}
