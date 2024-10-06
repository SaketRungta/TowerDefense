
#include "Projectile/SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Towers/SBaseTower.h"

ASProjectile::ASProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneRoot);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 2800.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnProjectileHit);
}

void ASProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DeactivateThisObject();
}

void ASProjectile::ActivateThisObject(const ASBaseTower* OwningTower)
{
	TurretSocketTransform = OwningTower->GetTurret()->GetSocketTransform(FName("ProjectileFire"));

	bIsInUse = true;

	SetActorTransform(TurretSocketTransform);

	SetActorHiddenInGame(false);

	SetActorEnableCollision(true);

	ProjectileMovementComponent->Velocity = GetActorForwardVector() * 1500.f;

	GetWorldTimerManager().SetTimer(
		DeactivateTimer,
		[this]()
		{
			if (bIsInUse) DeactivateThisObject();
		},
		5.f,
		false
		);
}

void ASProjectile::DeactivateThisObject()
{
	ProjectileMovementComponent->Velocity = FVector(0.f);

	SetActorEnableCollision(false);

	GetWorldTimerManager().ClearTimer(DeactivateTimer);

	bIsInUse = false;

	SetActorTransform(TurretSocketTransform);

	SetActorHiddenInGame(true);
}
