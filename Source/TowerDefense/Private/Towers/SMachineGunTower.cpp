
#include "Towers/SMachineGunTower.h"
#include "Projectile/SProjectile.h"

void ASMachineGunTower::FireTurret()
{
	Super::FireTurret();

	/**
	 * Not using as a smart pointer beacuse
	 * - Since the pointer is not stored or referenced elsewhere
	 * - Minimal overhead, as raw pointers most efficient access in performace critical sections like this
	 *
	 * TObjectPtr can be used when it would have been used in multiple functions
	 * TWeakObjectPtr as we are not destroying or deleting it here in this function
	 */
	ASProjectile* Projectile = nullptr;

	if (FindProjectileFromPool(Projectile))
	{
		static uint32 TurretToFireFrom = 0;
		FTransform ProjectileFireTransform = FTransform();

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
		}

		if (Projectile) Projectile->ActivateThisObject(ProjectileFireTransform);
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ASMachineGunTower::FireTurret Projectile is nullptr"));
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASMachineGunTower::FireTurret Projectile is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASMachineGunTower::FireTurret Projectile pool is empty"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASMachineGunTower::FireTurret Projectile pool is empty"));
	}
}
