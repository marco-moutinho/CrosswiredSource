// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathComponent.h"

// Sets default values for this component's properties
UPatrolPathComponent::UPatrolPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPatrolPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Function_SetPath();
}


// Called every frame
void UPatrolPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPatrolPathComponent::Function_SetPath()
{
	// Safety check
	if (_TargetPathPointptr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_SetPath() - _TargetPathPointptr = nullptr"))
	}
	// set first PathPoint ptr
	_TargetPathPointptr =_Pathptr->Function_GetFirstPathPoint();
}

void UPatrolPathComponent::Function_AskForNextPathPoint()
{
	// Safety Check : if is nullptr can be:
	if (_TargetPathPointptr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_AskForNextPathPoint() - STEP 0 - _TargetPathPointptr = nullptr"))
	}

	APathPoint* LcCurrent;
	LcCurrent = _TargetPathPointptr;
	_TargetPathPointptr = _Pathptr->Function_GetNextPathPoint(LcCurrent);

	// safety check
	if (_TargetPathPointptr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_AskForNextPathPoint() - _TargetPathPointptr = nullptr"))
	}
}

FVector UPatrolPathComponent::Function_AskForNextPathPointLocation()
{
	Function_AskForNextPathPoint();
	return _TargetPathPointptr->GetActorLocation();
}

