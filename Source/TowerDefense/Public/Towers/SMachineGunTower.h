
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

protected:
	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual bool FireTurret() override;

	/** Activates the muzzle flash for the machine gun tower */
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateMuzzleFlash(const FName& SocketName = FName("ProjectileFire_Left"));
	
};
