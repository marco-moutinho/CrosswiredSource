// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Element.generated.h"
// created on 15-Mar-2026
/**
 * 
 */
UCLASS()
class MP_UTILITIES_API UPDA_Element : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Element Base Data")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Element Base Data")
	FName Name;
};
