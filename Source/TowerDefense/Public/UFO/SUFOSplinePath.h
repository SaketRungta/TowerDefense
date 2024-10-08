
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
	ASUFOSplinePath();

protected:
	TObjectPtr<USplineComponent> Spline;

public:
	FORCEINLINE const USplineComponent* GetSplinePath() const
	{
		return Spline;
	}

};
