
#include "Towers/SMachineGunTower.h"
#include "Projectile/SProjectile.h"

bool ASMachineGunTower::FireTurret()
{
	if (!Super::FireTurret()) return false;

	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		static FName SocketToFireFrom = FName("ProjectileFire_Left");

		if (SocketToFireFrom == FName("ProjectileFire_Left"))
			SocketToFireFrom = FName("ProjectileFire_Right");
		else
			SocketToFireFrom = FName("ProjectileFire_Left");
		
		const FTransform ProjectileFireTransform = GetTurretMesh()->GetSocketTransform(SocketToFireFrom);
		if (Projectile)
			Projectile->ActivateThisObject(ProjectileFireTransform);
		
		ActivateMuzzleFlash(SocketToFireFrom);

		return true;
	}
	
	return false;
}
