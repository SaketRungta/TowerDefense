
#include "Towers/SCatapultTower.h"
#include "Projectile/SProjectile.h"

ASCatapultTower::ASCatapultTower()
{
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
			UE_LOG(LogTemp, Error, TEXT("ASCatapultTower::FireTurret Projectile is nullptr"));
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASArcherTower::FireTurret Projectile is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASArcherTower::FireTurret Projectile pool is empty"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASArcherTower::FireTurret Projectile pool is empty"));
	}
}
