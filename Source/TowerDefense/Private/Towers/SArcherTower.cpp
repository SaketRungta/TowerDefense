
#include "Towers/SArcherTower.h"
#include "Projectile/SProjectile.h"

ASArcherTower::ASArcherTower()
{
	UpdateTowerDataFromDataTable(FName("ArcherTower"));
}

void ASArcherTower::FireTurret()
{
	Super::FireTurret();

	ASProjectile* Projectile = nullptr;
	if (FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
