
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFOSplinePath.generated.h"

class USplineComponent;

/**
 * Spline path that UFOs have to follow
 * Can be set in the level itself and assigned to the wave manager for the UFO to follow
 ********************************************************************************************/
UCLASS(BlueprintType, Blueprintable, Category = UFO)
class TOWERDEFENSE_API ASUFOSplinePath : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFOSplinePath();

private:
	/** Spline component for the UFO to follow */
	UPROPERTY(EditDefaultsOnly, category = Components)
	TObjectPtr<USplineComponent> Spline;

public:
	/** Getter for Spline, called by ASUFOWaveManager::SpawnUFOs, to assign the spawned ufo its spline */
	FORCEINLINE const USplineComponent* GetSplinePath() const
	{ return Spline; }
	
};
