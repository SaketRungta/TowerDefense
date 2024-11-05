
#include "Towers/SCannonTower.h"
#include "Projectile/SProjectile.h"

ASCannonTower::ASCannonTower()
{
	UpdateTowerDataFromDataTable(FName("CannonTower"));
}

void ASCannonTower::FireTurret()
{
	Super::FireTurret();
	
	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
