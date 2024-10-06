
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

	TurretRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBaseTower::OnTurretRangeSphereOverlap);
	TurretRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASBaseTower::OnTurretRangeSphereEndOverlap);
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

void ASBaseTower::OnTurretRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InRangeEnemies.Contains(OtherActor)) return;
	
	InRangeEnemies.Add(OtherActor);
	if (InRangeEnemies.Num() == 1)
	{
		SetActorTickEnabled(true);
		SetTurretLookAtEnemy();
		FireTurret();
	}
}

void ASBaseTower::OnTurretRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

	TurretMesh->SetWorldRotation(FRotator(0.f, TurretLookAtEnemyRotation.Yaw, 0.f));
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
		if(Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ASBaseTower::FireTurret Projectile is nullptr"));
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASBaseTower::FireTurret Projectile is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASBaseTower::FireTurret Projectile pool is empty"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ASBaseTower::FireTurret Projectile pool is empty"));
	}

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
