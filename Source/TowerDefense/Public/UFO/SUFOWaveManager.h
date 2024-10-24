
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFOWaveManager.generated.h"

class ASUFO;
class ASUFOSplinePath;

/**
 * Struct to contain the data related the wave to spawn
 * 
 * @param UFOToSpawn: Class of the UFO to spawn
 * @param SpawnCount: Number of UFO to spawn
 * @param SplinePath: Spline path class, returns the spline path this UFO has to follow
 * @param SpawningRate: Time intervals in which UFO has to be spawned
 ********************************************************************************************/
USTRUCT(BlueprintType)
struct FWaveSpawnData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASUFO> UFOToSpawn;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "50"))
	uint32 SpawnCount = 1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ASUFOSplinePath> SplinePath;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "10"))
	float SpawningRate = 3.f;

};

/**
 * Struct to contain an array of wave spawn data
 * 
 * @param WaveData: Array of waves that are needed to be spawned currently all in one go
 * 
 * Notes:
 * - We are required to spawn multiple waves in one go, so we needed an array of array
 * - Unreal does not support such system so we have to make another struct
 ********************************************************************************************/
USTRUCT(BlueprintType)
struct FWaveDataArray
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FWaveSpawnData> WaveData;

};

/**
 * Spawns waves of UFOs
 * Data can be set from the level itself as every level waves will be custom
 ********************************************************************************************/
UCLASS(BlueprintType, Blueprintable, Category = UFO)
class TOWERDEFENSE_API ASUFOWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFOWaveManager();

protected:
	/** Begin play override */
	virtual void BeginPlay() override;

	/** Array containing the wave spawning data, to be set in BP */
	UPROPERTY(EditAnywhere, Category = WaveData)
	TArray<FWaveDataArray> WaveSpawningData;

	/** Time Intervals in which waves have to be spawned */
	UPROPERTY(EditAnywhere, Category = WaveData, meta = (ClampMin = "0.1", ClampMax = "10"))
	float WavesSpawningRate = 3.f;

private:
	/** Handles the functionality to spawn the next wave, called by ASUFOWaveManager::CompletedSpawningWave and BeginPlay */
	void SpawnNextWave();

	/** Checks if the given wave data is valid, if true then calls SpawnUFOs to spawn the actual wave */
	void CheckAndSpawnTheWaveWithGivenData(const FWaveSpawnData& InWaveSpawnData);

	/** Spawns the UFOs with the given data */
	void SpawnUFOs(FWaveSpawnData InWaveSpawnData, uint32 InNumUFOSpawned = 0);

	/** Checks if all the waves have been spawned, if true then calls to spawn the next wave */
	void CheckAndSpawnNextWave();

	/** Index of the currently spawning wave */
	uint32 CurrentWaveIndex = 0;

	/** Total number of sub waves that are required to spawn in the current wave */
	uint32 NumWavesToSpawn = 0;

	/** Number of sub waves that have been spawned from the current wave */
	uint32 NumWavesSpawned = 0;

};
