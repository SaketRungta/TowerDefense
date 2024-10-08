
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
	
	StartSpawningNextWave();
}

void ASUFOWaveManager::StartSpawningNextWave()
{
	CurrentWaveIndex += 1;

	SpawnUFOs();
}

void ASUFOWaveManager::SpawnUFOs()
{
	if (!WaveSpawningData.IsValidIndex(CurrentWaveIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("<===ERROR===> ASUFOWaveManager::SpawnUFOs is not valid index: %d"), CurrentWaveIndex);
		return;
	}

	if (CurrentWaveSpawnCount >= WaveSpawningData[CurrentWaveIndex].SpawnCount)
	{
		CurrentWaveSpawnCount = 0;
		StartSpawningNextWave();
		return;
	}

	if (!WaveSpawningData[CurrentWaveIndex].UFOToSpawn->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("<===ERROR===> ASUFOWaveManager::SpawnUFOs UFOToSpawn is not valid class index: %d"), CurrentWaveIndex);
		return;
	}

	ASUFO* SpawnedUFO = GetWorld()->SpawnActor<ASUFO>(
		WaveSpawningData[CurrentWaveIndex].UFOToSpawn,
		GetActorTransform()
	);

	if (SpawnedUFO && WaveSpawningData[CurrentWaveIndex].SplinePath->IsValidLowLevel())
	{
		SpawnedUFO->MoveAlongSplinePath(WaveSpawningData[CurrentWaveIndex].SplinePath->GetSplinePath());
	}

	CurrentWaveSpawnCount++;

	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&ASUFOWaveManager::SpawnUFOs,
		WaveSpawningData[CurrentWaveIndex].SpawningRate
	);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("ASUFOWaveManager::SpawnUFOs Spawned a UFO"));
}

