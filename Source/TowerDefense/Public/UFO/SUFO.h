
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFO.generated.h"

class USplineComponent;

/**
 * Base UFO class
 * UFO for the towers to shoot at
 ********************************************************************************************/
UCLASS(Abstract, BlueprintAble, BlueprintType, ClassGroup = UFO)
class TOWERDEFENSE_API ASUFO : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFO();

	/** Called when all the actor components have been initialized */
	virtual void PostInitializeComponents() override;

protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Main mesh of the UFO also acts as root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> UFOMesh = nullptr;

public:
	/** Implemented in BP makes the UFO move along the designated spline path */
	UFUNCTION(BlueprintImplementableEvent)
	void MoveAlongSplinePath(const USplineComponent* SplinePath);
};
