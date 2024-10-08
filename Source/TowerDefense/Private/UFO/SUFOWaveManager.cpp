
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
	if (!WaveSpawningData.IsValidIndex(CurrentWaveIndex))
	{
		FString Temp = FString::Printf(TEXT("ASUFOWaveManager::SpawnNextWave WaveSpawningData is not valid index: %d"), CurrentWaveIndex);
		UE_LOG(LogTemp, Error, TEXT("%s"), *Temp);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Temp);
		return;
	}

	FString Temp = FString::Printf(TEXT("ASUFOWaveManager::SpawnNextWave Spawning new wave"));
	UE_LOG(LogTemp, Log, TEXT("%s"), *Temp);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Temp);
	
	TArray<FWaveSpawnData> CurrentWaveData = WaveSpawningData[CurrentWaveIndex].WaveData;

	NumWavesToSpawn = CurrentWaveData.Num();

	for (int32 i = 0; i < CurrentWaveData.Num(); i++)
	{
		CheckAndSpawnTheWaveWithGivenData(CurrentWaveData[i]);
	}
}

void ASUFOWaveManager::CheckAndSpawnTheWaveWithGivenData(FWaveSpawnData& InWaveSpawnData)
{
	if (InWaveSpawnData.SpawnCount <= 0 || !InWaveSpawnData.UFOToSpawn->IsValidLowLevel() || !InWaveSpawnData.SplinePath->IsValidLowLevel())
	{
		FString Temp = FString::Printf(TEXT("ASUFOWaveManager::SpawnTheWaveWithGivenData UFOToSpawn/SplinePath is not valid or spawn count is zero"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *Temp);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Temp);

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
