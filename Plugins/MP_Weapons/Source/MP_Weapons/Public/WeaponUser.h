// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"

#include "WeaponUser.generated.h"

class UWeaponDefinitionPDA;
class AWeaponBase;
/**
 * Created on 12/12/2025
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MP_WEAPONS_API UWeaponUser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponUser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


// Weapon User UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs | UPROPERTYs |
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Flow")
	bool bAutomaticSetup = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Gameplay")
	TObjectPtr<UWeaponDefinitionPDA> StarterWeaponDef;

	UPROPERTY(BlueprintReadWrite, Category = "[ Weapon User RTO vars ]")
	TObjectPtr<AWeaponBase> CurrentWeaponPtr;// should i change this to other variable type?

	UPROPERTY(BlueprintReadWrite, Category = "[ Weapon User RTO vars ]")
	TObjectPtr<UCameraComponent> CameraComponentPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Gameplay|Trace")
	float POVTraceRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Gameplay|Trace")
	float PovTraceSphereRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Gameplay|Trace")
	FCollisionProfileName PovTraceProfile;

	UPROPERTY(BlueprintReadWrite, Category = "[ Weapon User RTO vars ]")
	FHitResult TraceResult;

	UPROPERTY()
	AActor* OwningActorPtr;

	// DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS | DEBUG DRAWS |
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Enable Debug Draws"))
	bool bDebug_EnableTraceDraws = false;

	/// [ Arrow ]
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Arrow Color"))
	FColor Debug_TraceArrowColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Arrow Thickness"))
	float Debug_ArrowThickness = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Arrow Depth"))
	int Debug_ArrowDepth = 1;
	//... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
	/// [ Sweep Sphere ]
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sweep Color"))
	FColor Debug_TraceSweepColor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sweep Draw Times"))
	int Debug_SphereSweepTimes = 9;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sphere Sweep Thickness"))
	float Debug_SphereSweepThickness = 0.5;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sphere Sweep Depth"))
	int Debug_SphereSweepDepth = 0;
	//... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
	/// [ Start/End ]
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "End Color"))
	FColor Debug_TraceEndColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sphere Start Thickness"))
	float Debug_SphereStartThickness = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sphere Start Depth"))
	int Debug_SphereStartDepth = 0;
	//... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
	/// Hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Hit Color"))
	FColor Debug_TraceHitColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Hit Point Size"))
	float Debug_HitPointSize = 20.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Hit Depth"))
	int Debug_HitDepth = 2;

	//... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
	/// Others
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Sphere Segments"))
	int Debug_SphereSegments = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "[ Weapon User Debug ]", meta = (DisplayName = "Draw Time"))
	float Debug_DrawTime = 3.0f;


	// Weapon User UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS | UFUNCTIONS |


	/**
	* Description:
	* This function it will be called on BeginPlay() if the bAutomaticSetup is true;
	* This function can find the owner camera component without any input parameters;
	* Created on 16/12/2025
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	void Function_AutoSetCameraComponentPointer();

	/// <summary>
	/// This is the "manual" version of set the CameraComponentPtr of this component;
	/// It needs to be called if bAutomaticSetup is false, It needs to be called after the owner character camera component is initialized;
	/// Created on 16/12/2025
	/// </summary>
	/// <param name="InPointer"></param>
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	void Function_ManualSetCameraComponentPointer(UCameraComponent* InPointer);

public:
	/*UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_TraceFromCamera(FHitResult& OutHitResult, FVector& OutTraceStart, FVector& OutTraceEnd);*/


	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]") // Added on 12-Jan-2026 / Last changed on 13-Jan-2026
	virtual void Function_SpawnWeaponFromDefinition(UWeaponDefinitionPDA* InWeaponDef, AWeaponBase*& OutSpawnedWeaponPtr);

	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]") // Added on 13-Jan-2026 / Last changed on 13-Jan-2026
	virtual void Function_AttachWeaponToHands(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent);

	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_UnequipCurrentWeapon();

	/*UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virutal void Function_SwitchWeapon();*/

	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_UseCurrentWeapon();

	/*
	* Performs a trace from player character aka pov
	* Created on 17/12/2025
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_TraceFromPov(FHitResult& OutHitResult);
};
