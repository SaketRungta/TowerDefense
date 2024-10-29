
#include "Towers/SCannonTower.h"
#include "Projectile/SProjectile.h"

void ASCannonTower::FireTurret()
{
	Super::FireTurret();

	ASProjectile* Projectile = nullptr;
	if (FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
