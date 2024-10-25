
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

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(TowerMesh, FName("BaseAttachment"));

	TowerRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TowerRangeSphere"));
	TowerRangeSphere->SetupAttachment(SceneRoot);
	TowerRangeSphere->SetSphereRadius(50.f);
	TowerRangeSphere->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	TowerRangeIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerRangeIndicatorMesh"));
	TowerRangeIndicatorMesh->SetupAttachment(TowerRangeSphere);
	TowerRangeIndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TowerRangeIndicatorMesh->SetHiddenInGame(true);

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		TowerRangeIndicatorMesh->SetStaticMesh(asset.Object);
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

	TowerRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBaseTower::OnTowerRangeSphereOverlap);
	TowerRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASBaseTower::OnTowerRangeSphereEndOverlap);

	OnBeginCursorOver.AddDynamic(this, &ASBaseTower::OnActorBeginCursorOver);
	OnEndCursorOver.AddDynamic(this, &ASBaseTower::OnActorEndCursorOver);
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

void ASBaseTower::OnActorBeginCursorOver(AActor* TouchedActor)
{
	TowerRangeIndicatorMesh->SetHiddenInGame(false);
	SetTowerEmissiveValue(0.5f);
}

void ASBaseTower::OnActorEndCursorOver(AActor* TouchedActor)
{
	TowerRangeIndicatorMesh->SetHiddenInGame(true);
	SetTowerEmissiveValue(0.f);
}

void ASBaseTower::OnTowerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InRangeEnemies.Contains(OtherActor) || !OtherActor->ActorHasTag(FName(TEXT("UFO")))) return;
	
	InRangeEnemies.Add(OtherActor);
	if (InRangeEnemies.Num() == 1)
	{
		SetActorTickEnabled(true);
		SetTurretLookAtEnemy();
		FireTurret();
	}
}

void ASBaseTower::OnTowerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!InRangeEnemies.Contains(OtherActor)) return;

	InRangeEnemies.Remove(OtherActor);
	if (InRangeEnemies.Num() == 0)
	{
		SetActorTickEnabled(false);
		GetWorldTimerManager().ClearTimer(FireCooldownTimer);
	}
}

void ASBaseTower::SetTurretLookAtEnemy()
{
	FRotator TurretLookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), InRangeEnemies[0]->GetActorLocation());

	FRotator TurretMeshCurrentRotation = TurretMesh->GetComponentRotation();
	TurretMesh->SetWorldRotation(FRotator(TurretMeshCurrentRotation.Pitch, TurretLookAtEnemyRotation.Yaw, TurretMeshCurrentRotation.Roll));
}

void ASBaseTower::SpawnProjectilePool()
{
	for (uint32 i = 0; i < ProjectilePoolSize; i++)
	{
		ASProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ASProjectile>(
			ProjectileClass,
			TurretMesh->GetSocketTransform(FName("ProjectileFire"))
		);
		
		if (SpawnedProjectile) ProjectilePool.Add(SpawnedProjectile);
	}
}

void ASBaseTower::FireTurret()
{
	GetWorldTimerManager().ClearTimer(FireCooldownTimer);

	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
		this,
		&ASBaseTower::FireTurret,
		FireRate
	);
}

bool ASBaseTower::FindProjectileFromPool(ASProjectile*& InProjectileRef)
{
	for (ASProjectile* CurrProjectile : ProjectilePool)
	{
		if (!CurrProjectile->IsProjectileInUse())
		{
			InProjectileRef = CurrProjectile;
			return true;
		}
	}

	return false;
}
