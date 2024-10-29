
#include "Towers/SArcherTower.h"
#include "Projectile/SProjectile.h"

void ASArcherTower::FireTurret()
{
	Super::FireTurret();

	ASProjectile* Projectile = nullptr;
	if (FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
