
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFO.generated.h"

class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUFODestroyed, uint32, UFOValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUFOReachedBase, uint32, UFOLifeCount);

/**
 * Base UFO class
 * UFO for the towers to shoot at
 ********************************************************************************************/
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UFO)
class TOWERDEFENSE_API ASUFO : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFO();

	/** When UFO is destroyed the player this delegate will tell the game mode its value to add to coin stash */
	FOnUFODestroyed OnUFODestroyed;
	
	/** When UFO reaches end this delegate will tell the game mode to deduct a life */
	FOnUFOReachedBase OnUFOReachedBase;
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

private:
	/** Main mesh of the UFO also acts as root component */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> UFOMesh = nullptr;

	/** Stores the health of UFO */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "100"))
	int32 Health = 5;

	/** Callback when projectile hit the ufo and applies damage */
	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	/** Implemented in BP makes the UFO move along the designated spline path */
	UFUNCTION(BlueprintImplementableEvent)
	void MoveAlongSplinePath(const USplineComponent* SplinePath);
};
