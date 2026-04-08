// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAttributes.h"

#include "Damageable.h"

// Sets default values for this component's properties
UGameplayAttributes::UGameplayAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGameplayAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGameplayAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGameplayAttributes::Function_TakeDamage(const float InDamage)
{
	float LDmg = InDamage;// - Resistence;
	if (LDmg <= 0.0f)
	{
		return;
	}
	else
	{
		Function_DecreaseHealth(LDmg);
	}
}

void UGameplayAttributes::Function_DecreaseHealth(const float InDelta)
{
	Health -= InDelta;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		Function_OnHealthEnded();
	}
}

void UGameplayAttributes::Function_IncreaseHealth(const float InDelta)
{
	Health += InDelta;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void UGameplayAttributes::Function_IncreaseMaxHealth(const float InDelta)
{
	MaxHealth += InDelta;
}

void UGameplayAttributes::Function_DecreaseMaxHealth(const float InDelta)
{
	MaxHealth -= InDelta;
}

void UGameplayAttributes::Function_OnHealthEnded()
{
	UE_LOG(LogTemp, Display, TEXT("[MARCO] : void UGameplayAttributes::Function_OnHealthEnded() : First line..."))
	if (GetOwner()->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[MARCO] : void UGameplayAttributes::Function_OnHealthEnded() : Owner Actor Class does NOT implements interface - UDamageable"))
	}
}

