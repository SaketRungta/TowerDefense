
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SCannonTower.generated.h"

/**
 * Cannon tower
 * Fire single rapid shots dealing small amounts of damage
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASCannonTower : public ASBaseTower
{
	GENERATED_BODY()

protected:
	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual bool FireTurret() override;

	/** Activates the muzzle flash for the cannon tower */
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateMuzzleFlash();
	
};
