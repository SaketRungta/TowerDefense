
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
	const int32 Seed = GetUniqueID() + FDateTime::Now().GetMillisecond();
	const FRandomStream RandomStream(Seed);
	const int32 PrefabIndex = RandomStream.RandRange(0, PrefabMeshes.Num() - 1);

	if (PrefabMeshes.IsValidIndex(PrefabIndex))
		PrefabMeshComponent->SetStaticMesh(PrefabMeshes[PrefabIndex]);
	
	const int32 ActorRotationYaw = RandomStream.RandRange(0, 3);
	PrefabMeshComponent->SetWorldRotation(FRotator(0.f, ActorRotationYaw, 0.f));
}
