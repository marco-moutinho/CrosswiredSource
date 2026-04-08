// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAttributes.generated.h"

USTRUCT()
struct FElementalAffinity
{
	GENERATED_BODY()
	
	public:
	UPROPERTY()
	float Fire;
	UPROPERTY()
	float Electric;
	UPROPERTY()
	float Poison;
	UPROPERTY()
	float Ice;
};

UENUM(BlueprintType)
enum class EElement : uint8
{
	Normal,
	Fire,
	Electric,
	Poison,
	Acid,
	Water,
	Ice,
	Rock,
	Steel,
	Plasma,
	Wood,
	Air,
	Sound,
	Dark,
	Lazer,
};

// created on 14-Mar-2026
// criei este component para uso mais simplificado pois o GameplatStats estava a dar demasiado trabalho por agora por ser um componente mais ambicioso / flexivel
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_STATS_API UGameplayAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayAttributes();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float Health;
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float Stamina;
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float MaxStamina;
	
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float Aether;
	UPROPERTY(BlueprintReadOnly, Category = "[ GAttributes ]|Base")
	float MaxAether;
	
	UPROPERTY()
	FElementalAffinity ResistenceAffinity;
	
public:
	// added on 14-Mar-2026 // the idea is so that this function is called by the owner, problaply having an interface to receive damage
	UFUNCTION(BlueprintCallable, Category = "[ Gameplay Attribute ]|Base")
	virtual void Function_TakeDamage(const float InDamage);
	
	// added on 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GAttribute Functions ]|Health")
	virtual void Function_DecreaseHealth(const float InDelta);
	
	// added on 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GAttribute Functions ]|Health")
	virtual void Function_IncreaseHealth(const float InDelta);
	
	// added on 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GAttribute Functions ]|Health")
	virtual void Function_IncreaseMaxHealth(const float InDelta);
	
	// added on 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GAttribute Functions ]|Base")
	virtual void Function_DecreaseMaxHealth(const float InDelta);
	
	// added on 14-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ GAttribute Functions ]|Base")
	virtual void Function_OnHealthEnded();
};
