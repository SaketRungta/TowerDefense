
#include "UFO/SUFOWaveManager.h"

#include "Components/SplineComponent.h"
#include "GameFramework/HUD.h"
#include "GameMode/SBaseGameMode.h"
#include "System/CommonTypes.h"
#include "UFO/SUFO.h"
#include "UFO/SUFOSplinePath.h"
#include "UI/SBaseHUD.h"

ASUFOWaveManager::ASUFOWaveManager()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ASUFOWaveManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (UFOsToMove.Num() > 0) MoveAllUFOsAlongTheSplinePath(DeltaSeconds);
	if (bAllTheUFOsHaveBeenSpawned && UFOsToMove.Num() == 0)
	{
		bAllTheUFOsHaveBeenSpawned = false;
		
		if (const ASBaseHUD* BaseHUD = Cast<ASBaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			BaseHUD->ShowTheGivenMenu(EMenuToShow::LevelCompleted);

		SetActorTickEnabled(false);
	}
}

void ASUFOWaveManager::BeginDestroy()
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(this);
	}
	
	Super::BeginDestroy();
}

void ASUFOWaveManager::BeginPlay()
{
	Super::BeginPlay();

	BaseGameMode = Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());
	if(BaseGameMode.IsValid()) BaseGameMode->SetTotalNumWaves(WaveSpawningData.Num());

	GetWorldTimerManager().SetTimer(
		StartSpawningWaveTimer,
		[this]()
		{
			SpawnNextWave();
		},
		2.f,
		false
	);
}

void ASUFOWaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(UFOSpawnTimer);
		TimerManager.ClearTimer(SpawnNextWaveTimer);
		TimerManager.ClearTimer(StartSpawningWaveTimer);
		TimerManager.ClearAllTimersForObject(this);
	}

	Super::EndPlay(EndPlayReason);	
}

void ASUFOWaveManager::SpawnNextWave()
{
	if (!WaveSpawningData.IsValidIndex(CurrentWaveIndex))
	{
		bAllTheUFOsHaveBeenSpawned = true;
		return;
	}
	
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
		BaseGameMode = BaseGameMode.IsValid() ? BaseGameMode : TObjectPtr<ASBaseGameMode>(Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode()));
		if(BaseGameMode.IsValid())
		{
			SpawnedUFO->OnUFODestroyed.AddDynamic(BaseGameMode.Get(), &ASBaseGameMode::OnUFODestroyedCallback);
			SpawnedUFO->OnUFOReachedBase.AddDynamic(BaseGameMode.Get(), &ASBaseGameMode::OnUFOReachedBaseCallback);
		}
	}
	
	if (SpawnedUFO && InWaveSpawnData.SplinePath->IsValidLowLevel())
	{
		SpawnedUFO->SetSplinePath(InWaveSpawnData.SplinePath->GetSpline());
		SpawnedUFO->SetWaveManager(this);
	}

	UFOsToMove.Add(SpawnedUFO);
	
	InNumUFOSpawned += 1;
	
	GetWorldTimerManager().ClearTimer(UFOSpawnTimer);
	GetWorldTimerManager().SetTimer(
		UFOSpawnTimer,
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

		GetWorldTimerManager().ClearTimer(SpawnNextWaveTimer);
		GetWorldTimerManager().SetTimer(
			SpawnNextWaveTimer,
			this,
			&ThisClass::SpawnNextWave,
			WavesSpawningRate
		);
	}
}

void ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath(const float& DeltaSeconds)
{
	for (TWeakObjectPtr<ASUFO> CurrUFO : UFOsToMove)
		if (CurrUFO.IsValid() && CurrUFO->GetSplinePath())
		{
			CurrUFO->SetDistanceAlongSpline(CurrUFO->GetDistanceAlongSpline() + (CurrUFO->GetMovementSpeed() * DeltaSeconds));

			float SplineLength = CurrUFO->GetSplinePath()->GetSplineLength();			
			
			if(CurrUFO->GetDistanceAlongSpline() >= SplineLength)
			{
				UFOsToRemoveAfterIteration.Add(CurrUFO);
				CurrUFO->OnUFOReachedBaseCall();
				continue;
			}
			
			CurrUFO->SetDistanceAlongSpline(FMath::Fmod(CurrUFO->GetDistanceAlongSpline(), SplineLength));

			FVector NewLocation = CurrUFO->GetSplinePath()->GetLocationAtDistanceAlongSpline(CurrUFO->GetDistanceAlongSpline(), ESplineCoordinateSpace::World);
			CurrUFO->SetActorLocation(NewLocation);
		}

	for (TWeakObjectPtr<ASUFO> UFOToRemove : UFOsToRemoveAfterIteration)
		UFOsToMove.Remove(UFOToRemove);

	UFOsToRemoveAfterIteration.Empty();
}

void ASUFOWaveManager::SpawnedUFODestroyedByPlayerCallback(ASUFO* DestroyedUFO)
{
	UFOsToRemoveAfterIteration.Add(DestroyedUFO);
}
