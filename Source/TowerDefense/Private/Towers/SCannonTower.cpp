
#include "Towers/SCannonTower.h"
#include "Projectile/SProjectile.h"

bool ASCannonTower::FireTurret()
{
	if (!Super::FireTurret()) return false;
	
	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));

		ActivateMuzzleFlash();

		return true;
	}

	return false;
}
