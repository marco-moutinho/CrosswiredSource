// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommandReceptor.h"

// Sets default values for this component's properties
UAICommandReceptor::UAICommandReceptor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	UE_LOG(LogTemp, Display, TEXT("UAICommandReceptor::UAICommandReceptor()"));
}


// Called when the game starts
void UAICommandReceptor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Display, TEXT("void UAICommandReceptor::BeginPlay()"));
}


// Called every frame
void UAICommandReceptor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAICommandReceptor::Function_GoToLocation(FVector InLocation)
{
	UE_LOG(LogTemp, Display, TEXT("void UAICommandReceptor::Function_GoToLocation(FVector InLocation)"));

	GoToLocation = InLocation;
	// update BehaviorTree?
	// update Blackboard?
}

void UAICommandReceptor::Function_SetTargetActor(AActor* InActorptr)
{
	UE_LOG(LogTemp, Display, TEXT("void UAICommandReceptor::Function_SetTargetActor(AActor* InActorptr)"));

	TargetActor = InActorptr;
	// update BehaviorTree?
	// update Blackboard?
}

