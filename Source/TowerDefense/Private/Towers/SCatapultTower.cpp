
#include "Towers/SCatapultTower.h"
#include "Projectile/SProjectile.h"
#include "Components/SphereComponent.h"

ASCatapultTower::ASCatapultTower()
{
	GetTurretMesh()->SetRelativeLocation(FVector(0.f, 0.f, 48.f));

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	TurretBase->SetupAttachment(GetTowerMesh(), FName("BaseAttachment"));

	CatapultProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CatapultProjectileMesh"));
	CatapultProjectileMesh->SetupAttachment(GetTurretMesh(), FName("ProjectileFire"));

	TowerInnerRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TowerInnerRangeSphere"));
	TowerInnerRangeSphere->SetupAttachment(GetRootComponent());
	TowerInnerRangeSphere->SetSphereRadius(50.f);
	TowerInnerRangeSphere->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	TowerInnerRangeIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerInnerRangeIndicatorMesh"));
	TowerInnerRangeIndicatorMesh->SetupAttachment(TowerInnerRangeSphere);
	TowerInnerRangeIndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TowerInnerRangeIndicatorMesh->SetHiddenInGame(true);
	TowerInnerRangeIndicatorMesh->SetWorldScale3D(FVector(1.2f));
	
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
		if (Asset.Succeeded()) TowerInnerRangeIndicatorMesh->SetStaticMesh(Asset.Object);
	}
	
}

void ASCatapultTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretBase->SetRelativeRotation(FRotator(0.f, GetTurretMesh()->GetComponentRotation().Yaw, 0.f));
}

void ASCatapultTower::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TowerInnerRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASCatapultTower::OnTowerInnerRangeSphereOverlap);
	TowerInnerRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASCatapultTower::OnTowerInnerRangeSphereEndOverlap);
}

void ASCatapultTower::ShowInnerRangeIndicatorMesh(const bool bShowSphere) const
{
	TowerInnerRangeIndicatorMesh->SetHiddenInGame(!bShowSphere);
}

void ASCatapultTower::BeginPlay()
{
	Super::BeginPlay();

	TowerInnerRangeSphere->SetSphereRadius(TowerInnerRange);
	TowerInnerRangeIndicatorMesh->SetWorldScale3D(FVector((TowerInnerRange / 50.f) * TowerInnerRangeIndicatorMesh->GetComponentScale().X));
}

bool ASCatapultTower::FireTurret()
{
	if (!Super::FireTurret()) return false;

	FireCatapult();

	return true;
}

void ASCatapultTower::OnTowerInnerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!InRangeEnemies.Contains(OtherActor) || !OtherActor->ActorHasTag(FName(TEXT("UFO")))) return;

	InRangeEnemies[OtherActor] = false;
}

void ASCatapultTower::OnTowerInnerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!InRangeEnemies.Contains(OtherActor) || !OtherActor->ActorHasTag(FName(TEXT("UFO")))) return;

	InRangeEnemies[OtherActor] = true;
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

	if (ASProjectile* Projectile = nullptr; FindProjectileFromPool(Projectile))
	{
		if (Projectile) Projectile->ActivateThisObject(GetTurretMesh()->GetSocketTransform(FName("ProjectileFire")));
	}
}
