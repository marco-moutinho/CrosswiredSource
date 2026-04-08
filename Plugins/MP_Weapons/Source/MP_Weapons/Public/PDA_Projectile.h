// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Element.h"
#include "PDA_Projectile.generated.h"

// created on 15-Mar-2026
/**
 * 
 */
UCLASS(Blueprintable)
class MP_WEAPONS_API UPDA_Projectile : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Base Data")
	float Damage;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Base Data")
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Base Data")
	TSoftObjectPtr<UPDA_Element> ElementData;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile Base Data")
	TSoftObjectPtr<UTexture2D> Icon;
};
