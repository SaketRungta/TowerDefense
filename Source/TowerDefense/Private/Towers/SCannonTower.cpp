
#include "Towers/SCannonTower.h"
#include "Projectile/SProjectile.h"

ASCannonTower::ASCannonTower()
{
	UpdateTowerDataFromDataTable(FName("CannonTower"));
}

bool ASCannonTower::FireTurret()
{
	if (!Super::FireTurret()) return false;
	
	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}

	return true;
}
