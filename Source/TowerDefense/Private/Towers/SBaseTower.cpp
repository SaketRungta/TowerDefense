
#include "Towers/SBaseTower.h"

ASBaseTower::ASBaseTower()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(SceneRoot);
	TowerMesh->SetRelativeScale3D(FVector(20.f));

	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(TowerMesh, FName("BaseAttachment"));
	Turret->SetRelativeScale3D(FVector(1.2f));
}

void ASBaseTower::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBaseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

