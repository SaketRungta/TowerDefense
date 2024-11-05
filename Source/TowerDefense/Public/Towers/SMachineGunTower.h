
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SMachineGunTower.generated.h"

/**
 * Machine gun tower
 * Fires rapid rounds dealing small damage to UFOs
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASMachineGunTower : public ASBaseTower
{
	GENERATED_BODY()

public:
	/** Default Constructor */
	ASMachineGunTower();
	
protected:
	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret() override;
	
};
