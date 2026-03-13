// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Damage.generated.h"

// Created on 10-mar-2026
// should I just make damage types a data object (PrimaryDataAsset) ?
UENUM(BlueprintType)
enum class EDamageType : uint8 {
	Normal,
	Shock,
	Acid,
};

// Created on 10-mar-2026
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_DAMAGE_API UDamage : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		// added on 10-mar-2026
	virtual void Function_ReceiveDamage(AActor* InEmmiterActor, float InValue);
};
