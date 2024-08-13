
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SArcherTower.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ASArcherTower : public ASBaseTower
{
	GENERATED_BODY()

protected:

	virtual void FireTurret() override;

};
