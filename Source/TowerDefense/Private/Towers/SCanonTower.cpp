
#include "Towers/SCanonTower.h"
#include "Projectile/SProjectile.h"

void ASCanonTower::FireTurret()
{
	ASProjectile* Projectile = FindProjectileFromPool();

	if (Projectile)
	{
		Projectile->ActivateThisObject(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASCanonTower::FireTurret Projectile null"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASCanonTower::FireTurret Projectile null"));
	}

	GetWorldTimerManager().ClearTimer(FireCooldownTimer);

	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
		this,
		&ASCanonTower::FireTurret,
		FireRate
	);
}
