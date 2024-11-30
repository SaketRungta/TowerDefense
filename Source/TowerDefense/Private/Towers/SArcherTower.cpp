
#include "Towers/SArcherTower.h"
#include "Projectile/SProjectile.h"

bool ASArcherTower::FireTurret()
{
	if (!Super::FireTurret()) return false;

	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		if (Projectile)
			Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
		return true;
	}

	return false;
}
