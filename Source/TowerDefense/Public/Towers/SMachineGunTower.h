
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SMachineGunTower.generated.h"

/**
 * Machine gun tower
 * Fires rapid rounds dealing small damage to UFOs
 ********************************************************************************************/
UCLASS(BlueprintAble, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASMachineGunTower : public ASBaseTower
{
	GENERATED_BODY()

protected:
	/** Invoked from ASBaseTower::OnTurretRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret() override;
	
};
