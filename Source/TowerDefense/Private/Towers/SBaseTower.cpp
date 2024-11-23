
#include "Towers/SBaseTower.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Interactions/STowerSite.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pawn/STowerDefensePawn.h"
#include "Projectile/SProjectile.h"
#include "UI/STowerSellingButton.h"

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
	TowerSellingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TowerSellingWidget->SetDrawSize(FVector2D(84.f, 84.f));

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		if (Asset.Succeeded()) TowerRangeIndicatorMesh->SetStaticMesh(Asset.Object);
	}
	
	Tags.Add(FName("Tower"));

}

void ASBaseTower::Destroyed()
{
	if (TowerSite.IsValid()) TowerSite->SetIsSiteDisabled(false);

	for (auto& CurrProjectile : ProjectilePool)
		if (IsValid(CurrProjectile)) CurrProjectile->Destroy();

	Super::Destroyed();
}

void ASBaseTower::BeginDestroy()
{

	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(this);
	}

	Super::BeginDestroy();
}

void ASBaseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetTurretLookAtEnemy();
	FireTurret();
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

void ASBaseTower::UpdateTowerDataFromDataTable(const FName InTowerName)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("DataTable'/Game/TowerDefense/Blueprints/DataTable/DT_TowerData.DT_TowerData'"));
	if (Asset.Succeeded())
		if (const UDataTable* TowerDataTable = Asset.Object)
			if (const FTowerDataTableRow* RowData = TowerDataTable->FindRow<FTowerDataTableRow>(InTowerName, TEXT("")))
			{
				this->ProjectileClass = RowData->ProjectileClass;
				this->FireRate = RowData->FireRate;
				this->ProjectilePoolSize = RowData->ProjectilePoolSize;
				this->ProjectileBaseDamage = RowData->ProjectileBaseDamage;
				this->TowerSellingPrice = RowData->TowerSellingPrice;
			}
}

void ASBaseTower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (ProjectileClass) SpawnProjectilePool();

	if (USTowerSellingButton* TowerSellingButton = Cast<USTowerSellingButton>(TowerSellingWidget->GetWidget()))
	{
		TowerSellingButton->SetOwningTower(this);
		TowerSellingButton->SetTowerSellingPrice(TowerSellingPrice);
	}
}

void ASBaseTower::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(FireCooldownTimer);
		TimerManager.ClearAllTimersForObject(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ASBaseTower::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	ASBaseTower* TempTower = nullptr;
	
	if(bIsTowerSelected)
	{
		if (TowerSellingWidget && TowerRangeIndicatorMesh)
		{
			TowerSellingWidget->SetHiddenInGame(true);
			TowerRangeIndicatorMesh->SetHiddenInGame(true);
		}
		SetTowerEmissiveValue(0.f);
	}
	else
	{
		if (TowerSellingWidget && TowerRangeIndicatorMesh)
		{
			TowerSellingWidget->SetHiddenInGame(false);
			TowerRangeIndicatorMesh->SetHiddenInGame(false);
		}
		SetTowerEmissiveValue(0.5f);

		TempTower = this;
	}
	
	bIsTowerSelected = !bIsTowerSelected;

	PlayerPawn = PlayerPawn.IsValid() ? PlayerPawn : TObjectPtr<ASTowerDefensePawn>(Cast<ASTowerDefensePawn>(GetWorld()->GetFirstPlayerController()->GetPawn()));
	if (PlayerPawn.IsValid()) PlayerPawn->SetCurrentlySelectedTower(TempTower);
	
}

void ASBaseTower::OnTowerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InRangeEnemies.Contains(OtherActor) || !OtherActor->ActorHasTag(FName(TEXT("UFO")))) return;
	
	InRangeEnemies.Add(OtherActor);
	if (InRangeEnemies.Num() == 1)
	{
		SetActorTickEnabled(true);
		bShouldFire = true;
	}
}

void ASBaseTower::OnTowerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!InRangeEnemies.Contains(OtherActor)) return;

	InRangeEnemies.Remove(OtherActor);
	if (InRangeEnemies.Num() == 0)
	{
		SetActorTickEnabled(false);
		bShouldFire = false;
	}
}

void ASBaseTower::SetTurretLookAtEnemy()
{
	const FRotator TurretLookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), InRangeEnemies[0]->GetActorLocation());

	const FRotator TurretMeshCurrentRotation = TurretMesh->GetComponentRotation();
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
		
		if (SpawnedProjectile)
		{
			ProjectilePool.Add(SpawnedProjectile);
			SpawnedProjectile->SetBaseDamage(ProjectileBaseDamage);
		}
	}
}

void ASBaseTower::SetTowerSite(ASTowerSite* InTowerSite)
{
	TowerSite = InTowerSite;
}

bool ASBaseTower::FireTurret()
{
	if (!bShouldFire || GetWorldTimerManager().GetTimerElapsed(FireCooldownTimer) > 0.f) return false;

	bShouldFire = false;

	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
[this]()
		{
			bShouldFire = true;
		},
		FireRate,
false
		);

	return true;
}

void ASBaseTower::SetTowerEmissiveValue_Implementation(const float EmissiveValue)
{
	GetTowerMesh()->SetScalarParameterValueOnMaterials(FName("Emissive"), EmissiveValue);
	GetTurretMesh()->SetScalarParameterValueOnMaterials(FName("Emissive"), EmissiveValue);
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
