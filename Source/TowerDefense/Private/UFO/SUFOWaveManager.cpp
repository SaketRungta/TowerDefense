
#include "UFO/SUFOWaveManager.h"
#include "UFO/SUFO.h"
#include "UFO/SUFOSplinePath.h"

ASUFOWaveManager::ASUFOWaveManager()
{
 	PrimaryActorTick.bCanEverTick = false;

}

void ASUFOWaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnNextWave();
}

void ASUFOWaveManager::SpawnNextWave()
{
	if (!WaveSpawningData.IsValidIndex(CurrentWaveIndex)) return;
	
	TArray<FWaveSpawnData> CurrentWaveData = WaveSpawningData[CurrentWaveIndex].WaveData;

	NumWavesToSpawn = CurrentWaveData.Num();

	for (int32 i = 0; i < CurrentWaveData.Num(); i++)
	{
		CheckAndSpawnTheWaveWithGivenData(CurrentWaveData[i]);
	}
}

void ASUFOWaveManager::CheckAndSpawnTheWaveWithGivenData(const FWaveSpawnData& InWaveSpawnData)
{
	if (InWaveSpawnData.SpawnCount <= 0 || !InWaveSpawnData.UFOToSpawn->IsValidLowLevel() || !InWaveSpawnData.SplinePath->IsValidLowLevel())
	{
		CheckAndSpawnNextWave();
		return;
	}

	SpawnUFOs(InWaveSpawnData);
}

void ASUFOWaveManager::SpawnUFOs(FWaveSpawnData InWaveSpawnData, uint32 InNumUFOSpawned)
{
	if (InNumUFOSpawned >= InWaveSpawnData.SpawnCount)
	{
		CheckAndSpawnNextWave();
		return;
	}

	ASUFO* SpawnedUFO = GetWorld()->SpawnActor<ASUFO>(
		InWaveSpawnData.UFOToSpawn,
		GetActorTransform()
	);

	if (SpawnedUFO && InWaveSpawnData.SplinePath->IsValidLowLevel())
	{
		SpawnedUFO->MoveAlongSplinePath(InWaveSpawnData.SplinePath->GetSplinePath());
	}

	InNumUFOSpawned += 1;

	FTimerHandle SpawnTimer;
	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		[this, InWaveSpawnData, InNumUFOSpawned]()
		{
			SpawnUFOs(InWaveSpawnData, InNumUFOSpawned);
		},
		InWaveSpawnData.SpawningRate,
		false
	);
}

void ASUFOWaveManager::CheckAndSpawnNextWave()
{
	NumWavesSpawned += 1;

	if (NumWavesSpawned == NumWavesToSpawn)
	{
		CurrentWaveIndex += 1;
		NumWavesSpawned = 0;

		FTimerHandle SpawnNextWaveTimer;
		GetWorldTimerManager().SetTimer(
			SpawnNextWaveTimer,
			this,
			&ThisClass::SpawnNextWave,
			WavesSpawningRate
		);
	}
}
