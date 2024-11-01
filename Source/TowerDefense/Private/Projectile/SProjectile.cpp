
#include "Projectile/SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

	SetActorEnableCollision(false);
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorHiddenInGame(true);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnProjectileHit);
}

void ASProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor, static_cast<float>(BaseDamage), GetInstigatorController(), this, UDamageType::StaticClass());
	DeactivateThisObject();
}

void ASProjectile::ActivateThisObject(const FTransform& InFiringSocketTransform)
{
	OwningTurretFiringSocketTransform = InFiringSocketTransform;

	bIsInUse = true;

	SetActorTransform(OwningTurretFiringSocketTransform);

	SetActorHiddenInGame(false);

	SetActorEnableCollision(true);

	ProjectileMovementComponent->Velocity = GetActorForwardVector() * FiringSpeed;

	GetWorldTimerManager().SetTimer(
		DeactivateTimer,
		[this]()
		{
			if (bIsInUse) DeactivateThisObject();
		},
		DeactivationTime,
		false
		);
}

void ASProjectile::DeactivateThisObject()
{
	ProjectileMovementComponent->Velocity = FVector(0.f);

	SetActorEnableCollision(false);

	GetWorldTimerManager().ClearTimer(DeactivateTimer);

	bIsInUse = false;

	SetActorTransform(OwningTurretFiringSocketTransform);

	SetActorHiddenInGame(true);
}
