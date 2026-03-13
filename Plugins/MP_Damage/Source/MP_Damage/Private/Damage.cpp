// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage.h"

// Sets default values for this component's properties
UDamage::UDamage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamage::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Display, TEXT("void UDamage::BeginPlay()"))
	
}


// Called every frame
void UDamage::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamage::Function_ReceiveDamage(AActor* InEmmiterActor, float InValue)
{
	UE_LOG(LogTemp, Display, TEXT("void UDamage::Function_ReceiveDamage(AActor* InEmmiterActor, float InValue)"))
}

