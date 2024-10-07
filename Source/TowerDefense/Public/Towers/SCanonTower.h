
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SCanonTower.generated.h"

UCLASS(BlueprintAble, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASCanonTower : public ASBaseTower
{
	GENERATED_BODY()

protected:
	/** Invoked from ASBaseTower::OnTurretRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret() override;

};
