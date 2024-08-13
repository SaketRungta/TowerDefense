
#include "Towers/SArcherTower.h"
#include "Projectile/SProjectile.h"

void ASArcherTower::FireTurret()
{
	/*
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("<=== ERROR ===> ASArcherTower::FireTurret invalid ProjectileClass"));
		return;
	}

	ASProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ASProjectile>(
		ProjectileClass,
		Turret->GetSocketTransform(FName("ProjectileAttach"))
	);

	SpawnedProjectile->AttachToComponent(Turret, FAttachmentTransformRules::KeepWorldTransform, FName("ProjectileAttach"));

	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
		this,
		&ASArcherTower::FireTurret,
		FireRate
	);
	*/
}
