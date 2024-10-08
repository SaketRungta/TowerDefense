
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFOWaveManager.generated.h"

class ASUFO;
class ASUFOSplinePath;

USTRUCT(BlueprintType)
struct FWaveSpawnData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASUFO> UFOToSpawn;

	UPROPERTY(EditAnywhere)
	uint32 SpawnCount = 0;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ASUFOSplinePath> SplinePath;

	UPROPERTY(EditAnywhere)
	float SpawningRate = 3.f;

};

UCLASS(BlueprintType, BlueprintAble, Category = UFO)
class TOWERDEFENSE_API ASUFOWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASUFOWaveManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = WaveData)
	TArray<FWaveSpawnData> WaveSpawningData;

private:
	void StartSpawningNextWave();

	void SpawnUFOs();

	int32 CurrentWaveIndex = -1;

	uint32 CurrentWaveSpawnCount = 0;

	FTimerHandle SpawnTimer;
};
