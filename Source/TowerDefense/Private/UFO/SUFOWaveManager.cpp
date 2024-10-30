
#include "UFO/SUFOWaveManager.h"
#include "UFO/SUFO.h"
#include "UFO/SUFOSplinePath.h"
#include "GameMode/SBaseGameMode.h"

ASUFOWaveManager::ASUFOWaveManager()
{
 	PrimaryActorTick.bCanEverTick = false;

}

void ASUFOWaveManager::BeginPlay()
{
	Super::BeginPlay();

	BaseGameMode = Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());

	if(BaseGameMode.IsValid()) BaseGameMode->SetTotalNumWaves(WaveSpawningData.Num());

	SpawnNextWave();
}

void ASUFOWaveManager::SpawnNextWave()
{
	if (!WaveSpawningData.IsValidIndex(CurrentWaveIndex)) return;
	
	if(BaseGameMode.IsValid()) BaseGameMode->SetCurrentWaveNumber(CurrentWaveIndex + 1);
	
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

	if(SpawnedUFO)
	{
		BaseGameMode = BaseGameMode.IsValid() ? BaseGameMode : Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());
		if(BaseGameMode.IsValid())
		{
			SpawnedUFO->OnUFODestroyed.AddDynamic(BaseGameMode.Get(), &ASBaseGameMode::OnUFODestroyedCallback);
			SpawnedUFO->OnUFOReachedBase.AddDynamic(BaseGameMode.Get(), &ASBaseGameMode::OnUFOReachedBaseCallback);
		}
	}
	
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
