
#include "Towers/SBaseTower.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pawn/STowerDefensePawn.h"
#include "Projectile/SProjectile.h"
#include "Towers/SCatapultTower.h"
#include "Towers/STowerDataAsset.h"
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

	if (OrderedInRangeEnemies.Num() > 0 && SetTurretLookAtEnemy())
		FireTurret();
}

void ASBaseTower::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TowerRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASBaseTower::OnTowerRangeSphereEndOverlap);

	OnClicked.AddDynamic(this, &ASBaseTower::OnActorClicked);
}

void ASBaseTower::SetTowerToUnselected()
{
	TowerSellingWidget->SetHiddenInGame(true);
	TowerRangeIndicatorMesh->SetHiddenInGame(true);	 
	if (const ASCatapultTower* TempCatapultTower = Cast<ASCatapultTower>(this))
		TempCatapultTower->ShowInnerRangeIndicatorMesh(false);
	SetTowerEmissiveValue(0.f);

	bIsTowerSelected = false;
}

void ASBaseTower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	/**
	 * Reason for not binding in PostInitializeComponents
	 *
	 * When a tower is spawning where enemy is already in range then OnTowerRangeSphereOverlap calls SetActorTickEnabled(true)
	 * But OnTowerRangeSphereOverlap is executed before Beginplay when it was binded in PostInitializeComponents
	 * So when I moved the binding in here then tick is disabled at first then OnTowerRangeSphereOverlap is called
	 * Then it checks if enemy is in range then tick is enabled again
	 */
	TowerRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBaseTower::OnTowerRangeSphereOverlap);
	
	if (TowerDataAsset)
	{
		this->TowerRange = TowerDataAsset->TowerRange;
		{
			TowerRangeSphere->SetSphereRadius(TowerRange);
			TowerRangeIndicatorMesh->SetRelativeScale3D(FVector(TowerRange/50.f));
		}
		this->ProjectileClass = TowerDataAsset->ProjectileClass;
		this->FireRate = TowerDataAsset->FireRate;
		this->ProjectilePoolSize = TowerDataAsset->ProjectilePoolSize;
		this->ProjectileBaseDamage = TowerDataAsset->ProjectileBaseDamage;
		this->TowerSellingPrice = TowerDataAsset->TowerSellingPrice;
		this->TowerFiringSound = TowerDataAsset->TowerFiringSound;
		this->TowerClickedSound = TowerDataAsset->TowerClickedSound;
	}
	
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
	if (TowerClickedSound)
		UGameplayStatics::PlaySound2D(this, TowerClickedSound);
	
	ASBaseTower* TempTower = nullptr;

	const ASCatapultTower* TempCatapultTower = Cast<ASCatapultTower>(this);
	
	if(bIsTowerSelected)
	{
		if (TowerSellingWidget && TowerRangeIndicatorMesh)
		{
			TowerSellingWidget->SetHiddenInGame(true);
			TowerRangeIndicatorMesh->SetHiddenInGame(true);
		}
		if (TempCatapultTower) TempCatapultTower->ShowInnerRangeIndicatorMesh(false);
		SetTowerEmissiveValue(0.f);
	}
	else
	{
		if (TowerSellingWidget && TowerRangeIndicatorMesh)
		{
			TowerSellingWidget->SetHiddenInGame(false);
			TowerRangeIndicatorMesh->SetHiddenInGame(false);
		}
		if (TempCatapultTower) TempCatapultTower->ShowInnerRangeIndicatorMesh(true);
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

	InRangeEnemies.Add(OtherActor, true);
	OrderedInRangeEnemies.Add(OtherActor);
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
	OrderedInRangeEnemies.Remove(OtherActor);
	if (InRangeEnemies.Num() == 0)
	{
		SetActorTickEnabled(false);
		bShouldFire = false;
	}
}

bool ASBaseTower::SetTurretLookAtEnemy()
{
	AActor* EnemyToTarget = nullptr;
	if (InRangeEnemies[OrderedInRangeEnemies[0]])
	{
		EnemyToTarget = OrderedInRangeEnemies[0].Get();
	}

	/*
	for (const TTuple<TWeakObjectPtr<AActor>, bool> CurrentEnemy : InRangeEnemies)
	{
		if (InRangeEnemies[CurrentEnemy.Key])
		{
			EnemyToTarget = CurrentEnemy.Key.Get();
			int32 Index = OrderedInRangeEnemies.Find(EnemyToTarget);
			EnemyToTarget = OrderedInRangeEnemies[Index].Get();
			break;
		}			
	}
	*/

	if (EnemyToTarget)
	{
		const FRotator TurretLookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), EnemyToTarget->GetActorLocation());

		const FRotator TurretMeshCurrentRotation = TurretMesh->GetComponentRotation();
		TurretMesh->SetWorldRotation(FRotator(TurretMeshCurrentRotation.Pitch, TurretLookAtEnemyRotation.Yaw, TurretMeshCurrentRotation.Roll));

		return true;
	}
	
	return false;
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

	UGameplayStatics::PlaySoundAtLocation(this, TowerFiringSound, GetActorLocation());
	
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
