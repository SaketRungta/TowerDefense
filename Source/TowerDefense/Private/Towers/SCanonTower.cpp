
#include "Towers/SCanonTower.h"
#include "Projectile/SProjectile.h"

void ASCanonTower::FireTurret()
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
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ASCanonTower::FireTurret Projectile is nullptr"));
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASCanonTower::FireTurret Projectile is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASCanonTower::FireTurret Projectile pool is empty"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASCanonTower::FireTurret Projectile pool is empty"));
	}
}
