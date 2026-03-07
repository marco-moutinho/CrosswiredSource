// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

// created on 04-Mar-2026
UCLASS(Blueprintable)
class MP_WORLDINTERACT_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// added on 04-Mar-2026
	/*
	* This function is meant to be overrided on sub classes to define specific reactions
	*/
	UFUNCTION()
	virtual void Function_InteractWithMe(AActor* InActorPtr); // maybe a output to return mine specific class / ptr ? 
};
/// TO DO :
/// Create a  function to hightlight / make a effect of the material of the mesh of this actor, like to give feedback to the player that is interactable;
/// OR just play a particle effect like a shine to gain player attention;
/// 
/// a function to detect if the actors uses a static or skeletal mesh as visuals - probably needed to access its materal;
/// 
/// Interaction with me SOUND;
/// 
/// Function that calls behavior of me on being interacted