// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PatrolPath.h"
#include "PathPoint.h"

#include "PatrolPathComponent.generated.h"

// created on 08-Mar-2026
// this is my cpp version of : https://youtu.be/z8VJhDmAyx4?si=BP_Ua_er42Fg9OSw
// last change on...
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_NPC_API UPatrolPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolPathComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "[ Path ]")
	TSoftObjectPtr<APatrolPath> _Pathptr;

	UPROPERTY()
	APathPoint* _TargetPathPointptr;

public:
	// added on 08 - Mar - 2026
	/*
	* This is important cause I need to set the first PathPoint ptr if not it will crash
	*/
	UFUNCTION()
	virtual void Function_SetPath();

	// added on 08 - Mar - 2026
	/*
	* Asks to the _Path (actor) for a pointer to the next APathPoint
	*/
	UFUNCTION()
	virtual void Function_AskForNextPathPoint();

	// added on 08 - Mar - 2026
	UFUNCTION()
	virtual FVector Function_AskForNextPathPointLocation();
		
};
