// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponUser.generated.h"

class UWeaponDefinitionPDA;
class UProjectileWeaponPDA;
class AWeaponBase;

UENUM()
enum class EInputphase : uint8
{
	IP_OnStartPress	UMETA(DisplayName = "Start Press"),
	IP_OnBeingHeld	UMETA(DisplayName = "Held Button"),
	IP_OnBeingReleased UMETA(DisplayName = "Stop Press"),
};

/*
* To Holster =	   A) Character still own it;	  B) Still loaded in memory;
* Drop On Ground = A) Character no longer own it; B) Still loaded in memory;
* Destroy =		   A) Character no longer own it; B) No longer loaded in memory;
* Created on 15-Jan-2026
*/
UENUM(BlueprintType)
enum class EWeaponUnquipMode : uint8
{
	EWUM_ToHolster		UMETA(DisplayName = "To Holster", ToolTip = "A) Character still own it; B) Still loaded in memory;"),
	EWUM_DropOnGround	UMETA(DisplayName = "Drop On Ground", ToolTip = "A) Character no longer own it; B) Still loaded in memory;"),
	EWUM_Destroy		UMETA(DisplayName = "Destroy", ToolTip = "A) Character no longer own it; B) No longer loaded in memory;"),
};
//EWUM_ToInventory	UMETA(DisplayName = "To Inventory"),

USTRUCT(BlueprintType)
struct FHolsterSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	bool bSlotUnlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UWeaponDefinitionPDA> WeaponDefinition;

	UPROPERTY(BlueprintReadWrite)
	AWeaponBase* WeaponSpawnedPtr;

	// Add someting that represents weapon customization that the player did? Skins, attachments, mods, etc.
	// Should that be here?

	// Where is the weapon Ammo info stored?
};

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
	// PLACEHOLDER STUFF:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Placeholder")
	TObjectPtr<USceneComponent> PH_WeaponAttachPoint;
	// ...

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Weapon User Properties ]|Settings")
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
	UPROPERTY()
	APawn* OwningPawnPtr;

	UPROPERTY(BlueprintReadWrite, Category = "[ Weapon User RTO vars ]|Holdster")
	TObjectPtr<AWeaponBase> SecondaryWeaponPtr; // WIP NOT : Maybe this can be a ref to the previously equipped weapon so we can swap between two weapons quickly?

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "[ Weapon User RTO vars ]|Holdster")
	TArray<FHolsterSlot> Holster;

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
	/*
	* Performs a trace from player character aka pov
	* Created on 17/12/2025
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_TraceFromPov(FHitResult& OutHitResult);
	
	/*UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_TraceFromCamera(FHitResult& OutHitResult, FVector& OutTraceStart, FVector& OutTraceEnd);*/
	
	/*
	* It handles the complete process of spawning and equipping/attaching a weapon from the holster at the provided slot index;
	* It calls Function_SpawnWeaponFromHolster and Function_EquipWeapon internally;
	* Added on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_EquipWeaponAdvanced(int InHolsterSlotIndex, USceneComponent* InSceneComponent);

	/*
	* Spawns a weapon actor based on the weapon definition stored in the holster at the provided slot index;
	* If the weapon at the provided slot index is already spawned, it will return the existing reference;
	* It calls Function_SpawnWeaponFromDefinition internally (in case the weapon is not already spawned);
	* Created on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual AWeaponBase* Function_SpawnWeaponFromHolster(int InSlotIndex);

	/*
	* [ Weapon User Functions ]
	*  it "SpawnActorDeferred" and initializes it/passes it the initial setup properties
	* it's called internally by this->Function_SpawnWeaponFromHolster
	* Created on 12-Jan-2026 / Last changed on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual AWeaponBase* Function_SpawnWeaponFromDefinition(UWeaponDefinitionPDA* InWeaponDef);

	/*
	* Sets CurrentWeaponPtr to InWeaponRef;
	* Calls Function_AttachWeaponToHands internally;
	* Created on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_EquipWeapon(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent);

	/*
	* It is Called by Function_EquipWeapon internally; There aren't many use cases to call this directly from Blueprint;
	* It attaches to PH_WeaponAttachPoint by default;
	* Created on 13-Jan-2026 / Last changed on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_AttachWeaponToHands(AWeaponBase* InWeaponRef, USceneComponent* InSceneComponent);

	/*UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virutal void Function_SwitchWeapon();*/

	// Added on 14-Jan-2026 / Last changed on 15-Jan-2026
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_UseCurrentWeapon(EInputphase InInputphase);

	/*
	* Created on 15-Jan-2026 / Last changed on
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_UnequipCurrentWeapon(EWeaponUnquipMode InUWMode);

	/*
	* Created on 15-Jan-2026
	*/
	UFUNCTION(BlueprintCallable, Category = "[ Weapon User Functions ]")
	virtual void Function_StoreCurrentWeaponInHolster();
};


/// GPT Notes --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/// To do list:
/// Separate “spawn” from “equip” completely []
///      > Final mental model:
///		      SpawnWeapon(...)
///           EquipWeapon(...)
///           UnequipWeapon(...)
///	     > No function should:
///		      Spawn AND equip
///           Equip AND store
///           pawn AND select

/// [ Done ]
/// Output references are not reset on failure; If the function early-returns, OutSpawnedWeaponPtr contains garbage.
/// Same for: Function_SpawnWeaponFromHolster; Any similar pattern;
/// 
/// [ To Do ]
/// PH_WeaponAttachPoint can silently be null []
/// If this is not set in BP: Attach silently fails; Weapon appears at world origin or not at all;