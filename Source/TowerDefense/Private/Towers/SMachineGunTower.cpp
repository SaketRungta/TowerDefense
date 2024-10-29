
#include "Towers/SMachineGunTower.h"
#include "Projectile/SProjectile.h"

void ASMachineGunTower::FireTurret()
{
	Super::FireTurret();

	ASProjectile* Projectile = nullptr;
	if (FindProjectileFromPool(Projectile))
	{
		static uint32 TurretToFireFrom = 0;
		FTransform ProjectileFireTransform;

		switch (TurretToFireFrom)
		{
		case 0:
			ProjectileFireTransform = GetTurretMesh()->GetSocketTransform(FName("ProjectileFire_Left"));
			TurretToFireFrom = 1;
			break;
		case 1:
			ProjectileFireTransform = GetTurretMesh()->GetSocketTransform(FName("ProjectileFire_Right"));
			TurretToFireFrom = 0;
			break;
		default:
			ProjectileFireTransform = GetTurretMesh()->GetSocketTransform(FName("ProjectileFire_Left"));
			TurretToFireFrom = 1;
			break;			
		}

		if (Projectile) Projectile->ActivateThisObject(ProjectileFireTransform);
	}
}
