// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	_CanDraw = false;
	
}

// Called every frame
void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (GetWorld() && _bDebugDraw) {

		int32 LcIndex = 0;
		for (APathPoint* LcPathPointptr : _PathPoints) {

			// Get current Point
			APathPoint* LcCurrentPoint = _PathPoints[LcIndex];
			if (LcCurrentPoint == nullptr) { return; }

			// Get previous point
			if (LcIndex == 0) {

			}
			else
			{
				APathPoint* LcPreviousPoint = _PathPoints[LcIndex - 1];

				// if it is the second point draw Start Arrow
				if (LcIndex == 1) {
					DrawDebugDirectionalArrow(GetWorld(), LcPreviousPoint->GetActorLocation(), LcCurrentPoint->GetActorLocation(), _DirectionalArrowsSize, _StartDirectionalArrowColor, false, 0, _DirectionalArrowsDepth, _DirectionalArrowsThickness);
				}

				if (LcIndex != 1 && LcCurrentPoint != _PathPoints.Last()) {
					DrawDebugDirectionalArrow(GetWorld(), LcPreviousPoint->GetActorLocation(), LcCurrentPoint->GetActorLocation(), _DirectionalArrowsSize, _DirectionalArrowsColor, false, 0, _DirectionalArrowsDepth, _DirectionalArrowsThickness);
				}

				// if it is the last point
				if (LcCurrentPoint == _PathPoints.Last()) {

					// draw from penultimate to last
					DrawDebugDirectionalArrow(GetWorld(), LcPreviousPoint->GetActorLocation(), LcCurrentPoint->GetActorLocation(), _DirectionalArrowsSize, _EndDirectionalArrowColor, false, 0, _DirectionalArrowsDepth, _DirectionalArrowsThickness);

					// if it loops draw from LAST to FIRST
					if (bLoops) {
						DrawDebugDirectionalArrow(GetWorld(), LcCurrentPoint->GetActorLocation(), _PathPoints[0]->GetActorLocation(), _DirectionalArrowsSize, _EndDirectionalArrowColor, false, 0, _DirectionalArrowsDepth, _DirectionalArrowsThickness);
					}
				}
			}
			LcIndex++;
			
		}
	}
#endif
}

bool APatrolPath::ShouldTickIfViewportsOnly() const
{
	return _bDebugDraw;
}