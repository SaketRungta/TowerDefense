
#include "Environment/SRandomPrefabTile.h"

ASRandomPrefabTile::ASRandomPrefabTile()
{
 	PrimaryActorTick.bCanEverTick = false;

	PrefabMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrefabMeshComponent"));
	SetRootComponent(PrefabMeshComponent);
}

void ASRandomPrefabTile::BeginPlay()
{
	Super::BeginPlay();

	SetRandomPrefabTile();
}

void ASRandomPrefabTile::SetRandomPrefabTile()
{
	const int32 Seed = GetUniqueID() + static_cast<int32>(GetActorLocation().X + GetActorLocation().Y);
	const FRandomStream RandomStream(Seed);
	const int32 PrefabIndex = RandomStream.RandRange(0, PrefabMeshes.Num() - 1);

	if (PrefabMeshes.IsValidIndex(PrefabIndex))
		PrefabMeshComponent->SetStaticMesh(PrefabMeshes[PrefabIndex]);
	
	const int32 ActorRotationYaw = RandomStream.RandRange(0, 3);
	PrefabMeshComponent->SetWorldRotation(FRotator(0.f, ActorRotationYaw * 90.f, 0.f));
}
