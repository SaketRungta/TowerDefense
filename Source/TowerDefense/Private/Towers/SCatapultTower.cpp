
#include "Towers/SCatapultTower.h"
#include "Projectile/SProjectile.h"

ASCatapultTower::ASCatapultTower()
{
	UpdateTowerDataFromDataTable(FName("CatapultTower"));
	
	GetTurretMesh()->SetRelativeLocation(FVector(0.f, 0.f, 48.f));

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	TurretBase->SetupAttachment(GetTowerMesh(), FName("BaseAttachment"));

	CatapultProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CatapultProjectileMesh"));
	CatapultProjectileMesh->SetupAttachment(GetTurretMesh(), FName("ProjectileFire"));
}

void ASCatapultTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretBase->SetRelativeRotation(FRotator(0.f, GetTurretMesh()->GetComponentRotation().Yaw, 0.f));
}

void ASCatapultTower::FireTurret()
{
	Super::FireTurret();

	FireCatapult();
}

void ASCatapultTower::SetTurretMeshPitch(const float& InPitch)
{
	FRotator TurretMeshCurrentRotation = GetTurretMesh()->GetComponentRotation();
	TurretMeshCurrentRotation.Pitch = InPitch;
	GetTurretMesh()->SetWorldRotation(TurretMeshCurrentRotation);
}

void ASCatapultTower::FireProjectile()
{
	CatapultProjectileMesh->SetHiddenInGame(true);

	ASProjectile* Projectile = nullptr;
	if (FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
