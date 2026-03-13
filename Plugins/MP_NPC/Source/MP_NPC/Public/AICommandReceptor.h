// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AICommandReceptor.generated.h"

// created on 10-Mar-2026

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class MP_NPC_API UAICommandReceptor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAICommandReceptor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	FVector GoToLocation;

	UPROPERTY()
	TSoftObjectPtr<AActor> TargetActor;
public:

	// Added on 10-Mar-2026
	UFUNCTION(BlueprintCallable)
	virtual void Function_GoToLocation(FVector InLocation);

	// Added on 10-Mar-2026
	UFUNCTION(BlueprintCallable)
	virtual void Function_SetTargetActor(AActor* InActorptr);
		
};
