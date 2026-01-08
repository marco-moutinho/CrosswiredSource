// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"

#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetActorTickEnabled(false);

}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::Function_InitializeNPC()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, this->GetName() +  TEXT(" - ANPCBase::Function_InitializeNPC()"));
	}

	CharacterMovementComponentPtr = this->GetCharacterMovement();

	// defaults values by  Ryan Laley...
	bUseControllerRotationYaw = false;
	CharacterMovementComponentPtr->bUseControllerDesiredRotation = true;
	CharacterMovementComponentPtr->bOrientRotationToMovement = false;
	

}

