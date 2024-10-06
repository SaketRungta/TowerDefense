
#include "Towers/SBaseTower.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile/SProjectile.h"

ASBaseTower::ASBaseTower()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(SceneRoot);

	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(TowerMesh, FName("BaseAttachment"));

	TurretRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TurretRangeSphere"));
	TurretRangeSphere->SetupAttachment(SceneRoot);
	TurretRangeSphere->SetSphereRadius(50.f);
	TurretRangeSphere->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	TurretRangeIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretRangeIndicatorMesh"));
	TurretRangeIndicatorMesh->SetupAttachment(TurretRangeSphere);
	TurretRangeIndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Game/KenneyTowerDefenseKit/Assets/Models/towerRound_sampleA_towerRound_sampleA.towerRound_sampleA_towerRound_sampleA'"));
		TowerMesh->SetStaticMesh(asset.Object);
	}

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		TurretRangeIndicatorMesh->SetStaticMesh(asset.Object);
	}	
}

void ASBaseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetTurretLookAtEnemy();
}

void ASBaseTower::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TurretRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBaseTower::OnInTurretRangeBeginOverlap);
	TurretRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASBaseTower::OnInTurretRangeEndOverlap);
}

void ASBaseTower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (ProjectileClass) SpawnProjectilePool();
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASBaseTower::BeginPlay ProjectileClass null"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASBaseTower::BeginPlay ProjectileClass null"));
	}
}

void ASBaseTower::OnInTurretRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!InRangeEnemies.Contains(OtherActor))
	{
		InRangeEnemies.Add(OtherActor);
		if (InRangeEnemies.Num() == 1)
		{
			SetActorTickEnabled(true);
			SetTurretLookAtEnemy(); // Calling this before FireTurret() so that projectile move in the correct direction
			FireTurret();
		}
	}
}

void ASBaseTower::OnInTurretRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InRangeEnemies.Contains(OtherActor))
	{
		InRangeEnemies.Remove(OtherActor);
		if (InRangeEnemies.Num() == 0)
		{
			SetActorTickEnabled(false);
			GetWorldTimerManager().ClearTimer(FireCooldownTimer);
		}
	}
}
void ASBaseTower::SetTurretLookAtEnemy()
{
	FRotator TurretLookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(Turret->GetComponentLocation(), InRangeEnemies[0]->GetActorLocation());

	Turret->SetWorldRotation(FRotator(0.f, TurretLookAtEnemyRotation.Yaw, 0.f));
}

void ASBaseTower::SpawnProjectilePool()
{
	for (uint32 i = 0; i < ProjectilePoolSize; i++)
	{
		ASProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ASProjectile>(
			ProjectileClass,
			Turret->GetSocketTransform(FName("ProjectileFire"))
		);
		
		ProjectilePool.Add(SpawnedProjectile);
	}
}

ASProjectile* ASBaseTower::FindProjectileFromPool()
{
	for (ASProjectile* CurrProjectile : ProjectilePool)
	{
		if (!CurrProjectile->IsProjectileInUse())
		{
			return CurrProjectile;
		}
	}

	return nullptr;
}
