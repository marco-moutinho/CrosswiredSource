// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathComponent.h"

// Sets default values for this component's properties
UPatrolPathComponent::UPatrolPathComponent()
{
	UE_LOG(LogTemp, Display, TEXT("_> UPatrolPathComponent::UPatrolPathComponent()"));

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_bHasValidPath = false;

	// ...
}


// Called when the game starts
void UPatrolPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Display, TEXT("_> void UPatrolPathComponent::BeginPlay()"));
	Function_StartPath();
}


// Called every frame
void UPatrolPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPatrolPathComponent::Function_SetPath(APatrolPath* InPtr)
{
	UE_LOG(LogTemp, Display, TEXT("_> void UPatrolPathComponent::Function_SetPath(APatrolPath* InPtr)"));

	if (InPtr == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("_> UPatrolPathComponent::Function_SetPath(APatrolPath* InPtr) - InPtr is a nullptr"));
		_bHasValidPath = false;
		return;
	}

	_bHasValidPath = true;
	_Pathptr = InPtr;
}

void UPatrolPathComponent::Function_StartPath()
{
	UE_LOG(LogTemp, Display, TEXT("_> void UPatrolPathComponent::Function_StartPath()"));

	// Safety check
	if (_Pathptr == nullptr) {
		_bHasValidPath = false;
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_SetPath() - _Pathptr = nullptr"));
		return;
	}
	// set first PathPoint ptr
	_TargetPathPointptr =_Pathptr->Function_GetFirstPathPoint();

	// extra safety check
	if (_TargetPathPointptr == nullptr) {
		_bHasValidPath = false;
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_SetPath() -_TargetPathPointptr  = nullptr"));
	}

	_bHasValidPath = true;
}

void UPatrolPathComponent::Function_AskForNextPathPoint()
{
	UE_LOG(LogTemp, Display, TEXT("_> void UPatrolPathComponent::Function_AskForNextPathPoint()"));

	// Safety Check : if is nullptr can be:
	if (_TargetPathPointptr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_AskForNextPathPoint() - STEP 0 - _TargetPathPointptr = nullptr"));
		_bHasValidPath = false;
		return;
	}

	APathPoint* LcCurrent;
	LcCurrent = _TargetPathPointptr;
	_TargetPathPointptr = _Pathptr->Function_GetNextPathPoint(LcCurrent);
	_bHasValidPath = true;

	// safety check
	if (_TargetPathPointptr == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("_> UPatrolPathComponent::Function_AskForNextPathPoint() - _TargetPathPointptr = nullptr"));
		_bHasValidPath = false;
		return;
	}
}

FVector UPatrolPathComponent::Function_AskForNextPathPointLocation()
{
	UE_LOG(LogTemp, Display, TEXT("_> FVector UPatrolPathComponent::Function_AskForNextPathPointLocation()"));

	Function_AskForNextPathPoint();

	if (_bHasValidPath == false) {
		UE_LOG(LogTemp, Error, TEXT("_> FVector UPatrolPathComponent::Function_AskForNextPathPointLocation() - _bHasValidPath = false"));
	}

	return _TargetPathPointptr->GetActorLocation();
}

bool UPatrolPathComponent::Function_HasAValidPath()
{
	return _bHasValidPath;
}

