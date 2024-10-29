
#include "Towers/SBaseTower.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile/SProjectile.h"
#include "Components/WidgetComponent.h"
#include "Interface/SPlayerPawnInterface.h"

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

	TowerSellingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerSellingWidget"));
	TowerSellingWidget->SetupAttachment(SceneRoot);
	TowerSellingWidget->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	TowerSellingWidget->SetHiddenInGame(true);

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		TowerRangeIndicatorMesh->SetStaticMesh(asset.Object);
	}
	
	Tags.Add(FName("Tower"));
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

	OnClicked.AddDynamic(this, &ASBaseTower::OnActorClicked);
}

void ASBaseTower::SetTowerToUnselected()
{
	TowerSellingWidget->SetHiddenInGame(true);
	TowerRangeIndicatorMesh->SetHiddenInGame(true);
	SetTowerEmissiveValue(0.f);

	bIsTowerSelected = false;
}

void ASBaseTower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (ProjectileClass) SpawnProjectilePool();
}

void ASBaseTower::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	ASBaseTower* TempTower;
	
	if(bIsTowerSelected)
	{
		TowerSellingWidget->SetHiddenInGame(true);
		TowerRangeIndicatorMesh->SetHiddenInGame(true);
		SetTowerEmissiveValue(0.f);

		TempTower = nullptr;
	}
	else
	{
		TowerSellingWidget->SetHiddenInGame(false);
		TowerRangeIndicatorMesh->SetHiddenInGame(false);
		SetTowerEmissiveValue(0.5f);

		TempTower = this;
	}
	
	bIsTowerSelected = !bIsTowerSelected;

#pragma region InterfaceCall
	
	static APawn* PlayerPawn = nullptr;
	static ISPlayerPawnInterface* PlayerPawnInterface = nullptr;

	if (!PlayerPawn || !PlayerPawn->IsValidLowLevel()) PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (PlayerPawn && PlayerPawn->Implements<USPlayerPawnInterface>())
	{
		if (!PlayerPawnInterface) PlayerPawnInterface = Cast<ISPlayerPawnInterface>(PlayerPawn);
		if (PlayerPawnInterface) PlayerPawnInterface->SetCurrentlySelectedTower(TempTower);
	}
	
#pragma endregion InterfaceCall
	
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
