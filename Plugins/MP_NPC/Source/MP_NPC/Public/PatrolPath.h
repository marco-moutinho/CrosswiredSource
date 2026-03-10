// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PathPoint.h"

#include "PatrolPath.generated.h"
// created on 08-Mar-2026
UCLASS(Blueprintable, BlueprintType)
class MP_NPC_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]")
	TArray<APathPoint*> _PathPointsList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]")
	bool bLoops;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug")
	bool _bDebugDraw = false;

	UPROPERTY(BlueprintReadWrite, Category = "[ Path ]|Debug")
	bool _CanDraw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	float _DirectionalArrowsSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	uint8 _DirectionalArrowsDepth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	float _DirectionalArrowsThickness = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	FColor _DirectionalArrowsColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	FColor _StartDirectionalArrowColor = FColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[ Path ]|Debug|Directional Arrows")
	FColor _EndDirectionalArrowColor = FColor::Red;

public:
	// added on 08-Mar-2026
	UFUNCTION(BlueprintCallable, Category = "[ Path Function ]")
	virtual APathPoint* Function_GetNextPathPoint(APathPoint* InCurrentPathPointptr);

	// added on 08 - Mar - 2026
	UFUNCTION()
	virtual APathPoint* Function_GetFirstPathPoint();

	// added on 10-Mar-2026
	UFUNCTION()
	virtual bool Function_IsPathValid();

	/// TO DO : Add visual feedback to the "Game Designer" that the path is Invalid - like for exemple turn the PatrolPath Actor Red in invalid and Green if valid
};
