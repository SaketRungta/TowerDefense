
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFOSplinePath.generated.h"

class USplineComponent;

UCLASS(BlueprintType, BlueprintAble, Category = UFO)
class TOWERDEFENSE_API ASUFOSplinePath : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFOSplinePath();

protected:
	/** Spline component for the UFO to follow */
	UPROPERTY(EditAnywhere, category = Components)
	TObjectPtr<USplineComponent> Spline;

public:
	/** Getter for Spline, called by ASUFOWaveManager::SpawnUFOs, to assign the spawned ufo its spline */
	FORCEINLINE const USplineComponent* GetSplinePath() const
	{ return Spline; }

};
