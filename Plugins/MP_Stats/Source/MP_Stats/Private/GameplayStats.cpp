// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayStats.h"

// Sets default values for this component's properties
UGameplayStats::UGameplayStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameplayStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGameplayStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGameplayStats::Function_IncreaseTargetPropertyOfSelectedStat(const FGameplayTag& InStatTag, const EStatParamProperty InProperty, const float InDelta)
{
	// Safety Check
	if (FStatParams* LParams = Stats.Find(InStatTag))
	{
		switch (InProperty)
		{
			// Current param?
		case EStatParamProperty::Current:
			LParams->Current += InDelta;
			if (LParams->Current > LParams->Max)
			{
				LParams->Current = LParams->Max;
			}
			break;
			// max
		case EStatParamProperty::Max:
			LParams->Max += InDelta;
			break;
			// ragain rate
		case EStatParamProperty::Min:
			LParams->RegainRate += InDelta;
			break;
			// drain rate
			case EStatParamProperty::RegainRate:
			LParams->RegainRate += InDelta;
			break;
		} // END SWITCH
	}
	// degub
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[MARCO] void UGameplayStats::Function_IncreaseTargetPropertyOfSelectedStat() InStatTag NOT found !!!"));
	}
}

void UGameplayStats::Function_DecreaseTargetPropertyOfSelectedStat(const FGameplayTag InStatTag,
	const EStatParamProperty InProperty, const float InDelta)
{
	if (FStatParams* LParams = Stats.Find(InStatTag))
	{
		switch (InProperty)
		{
			case EStatParamProperty::Current:
			LParams->Current -= InDelta;
			if (LParams->Current < LParams->Min)
			{
				LParams->Current = LParams->Min;
			}
			break;
			
			case EStatParamProperty::Max:
			LParams->Max -= InDelta;
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[MARCO] void UGameplayStats::Function_DecreaseTargetPropertyOfSelectedStat(...) InStatTag NOT found !!!"));
	}
}


