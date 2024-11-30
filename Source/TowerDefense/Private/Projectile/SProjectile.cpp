
#include "Projectile/SProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

ASProjectile::ASProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneRoot);
	
	ProjectileTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileTrailComponent"));
	ProjectileTrailComponent->SetupAttachment(SceneRoot);
	ProjectileTrailComponent->SetAutoActivate(false);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	SetActorEnableCollision(false);
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorHiddenInGame(true);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnProjectileHit);
}

void ASProjectile::BeginDestroy()
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(this);
	}

	Super::BeginDestroy();
}

void ASProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCatapultProjectile)
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			GetWorld(),
			BaseDamage,
			0,
			GetActorLocation(),
			DamageInnerRadius,
			DamageOuterRadius,
			DamageFalloff,
			nullptr,
			{}
			);
	else
		UGameplayStatics::ApplyDamage(
			OtherActor,
			static_cast<float>(BaseDamage),
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
			);

	if (ProjectileHitSound)
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileHitSound, GetActorLocation());
	
	DeactivateThisObject();
}

void ASProjectile::ActivateThisObject(const FTransform& InFiringSocketTransform)
{
	if (ProjectileTrailComponent)
	{
		ProjectileTrailComponent->SetVisibility(true);
		ProjectileTrailComponent->Activate();
	}
	
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

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->InitialSpeed = FiringSpeed;
	ProjectileMovementComponent->MaxSpeed = FiringSpeed;
}

void ASProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(DeactivateTimer);
		TimerManager.ClearAllTimersForObject(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

void ASProjectile::DeactivateThisObject()
{
	if (ProjectileTrailComponent)
	{
		ProjectileTrailComponent->Deactivate();
		ProjectileTrailComponent->DeactivateImmediate();
		ProjectileTrailComponent->SetVisibility(false);
	}
	
	ProjectileMovementComponent->Velocity = FVector(0.f);
	
	SetActorEnableCollision(false);

	GetWorldTimerManager().ClearTimer(DeactivateTimer);

	bIsInUse = false;

	SetActorTransform(OwningTurretFiringSocketTransform);

	SetActorHiddenInGame(true);
}
