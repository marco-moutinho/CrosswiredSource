// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpStartController.h"

void AJumpStartController::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoInitialize)
	{
		// Initialize the Enhanced Input Subsystem
		Method_InitializeEnhancedInputSubsystem();

		// Initialize and add the main HUD to the viewport
		if (M_MainHudClass)
		{
			if (!IsValid(M_MainHudPtr))
			{
				M_MainHudPtr = CreateWidget<UWD_HUD>(this, M_MainHudClass);
				
				if (M_MainHudPtr)
				{
					M_MainHudPtr->AddToViewport();
					M_MainHudPtr->SetVisibility(ESlateVisibility::Visible);
					//M_MainHudPtr->Priority = 0; // Default priority
				}
			}
		}
	}
	
}

void AJumpStartController::Method_InitializeEnhancedInputSubsystem()
{
	/// Ok so this next line was tricky for me, what it does is that creates a pointer var that is set by the func "GetLocalPlayer",
	/// that can be null or not, so basicly the if checks if the pointer is null or not, a basic operation,
	/// Idk that in C++ its possible to declare and initialize a variable inside a if statement
	if (ULocalPlayer* L_LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = L_LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Check if the user/dev set the Input Mapping Context in the derived Blueprint
			if (M_IMC_Controls_Ptr)
			{
				Subsystem->AddMappingContext(M_IMC_Controls_Ptr, M_InputPriority);

				// Give feedback to the user/dev that the subsystem was initialized correctly
				if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Enhanced Input Subsystem Initialized"); }
			}
			else
			{
				// Give feedback to the user/dev that is missing the Input Mapping Context
				if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, "Input Mapping Context 'M_IMC_Controls_Ptr' is null, please set it on the JumpStartController derived Blueprint"); }
			}
		}
	}
}
