
#include "UFO/SUFOWaveManager.h"
#include "UFO/SUFO.h"

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

	GetWorld()->SpawnActor<ASUFO>(
		WaveSpawningData[CurrentWaveIndex].UFOToSpawn,
		GetActorTransform()
	);

	CurrentWaveSpawnCount++;

	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&ASUFOWaveManager::SpawnUFOs,
		WaveSpawningData[CurrentWaveIndex].SpawningRate
	);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("ASUFOWaveManager::SpawnUFOs Spawned a UFO"));
}

