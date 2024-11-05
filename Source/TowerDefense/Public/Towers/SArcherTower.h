
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SArcherTower.generated.h"

/**
 * Archer tower
 * Fires fast and rapid shots dealing good damage but expensive to buy
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASArcherTower : public ASBaseTower
{
	GENERATED_BODY()

public:
	/** Default Constructor */
	ASArcherTower();
	
protected:
	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret() override;

};
