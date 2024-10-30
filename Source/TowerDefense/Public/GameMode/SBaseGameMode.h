
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SBaseGameMode.generated.h"

/**
 * Base game mode class
 * Handles the life, coin count and wave count
 * When lives are over does the necessary functionality
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = GameMode)
class TOWERDEFENSE_API ASBaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	/**
	 * Callback when any UFO is destroyed by the player
	 * Adds the UFO value to the coin stash
	 * Wave manager class does the binding
	 */
	UFUNCTION()
	void OnUFODestroyedCallback(uint32 InUFOValue)
	{ CoinCount += InUFOValue; }
	
	/**
	 * Callback when any UFO is reaches the end
	 * Deducts life from the life count
	 * Checks if lives are over then ends the game
	 * Wave manager class does the binding
	 */
	UFUNCTION()
	void OnUFOReachedBaseCallback(uint32 InUFOLifeCount);
		
private:
	/** Stores the current count of coins */
	uint32 CoinCount = 91;

	/** Stores the current count of lives */
	uint32 LifeCount = 9991;

	/** Stores the total number of waves for the current level */
	uint32 TotalNumWaves = 91;

	/** Stores the currently active wave number */
	uint32 CurrentWaveNumber = 91;
	
public:
	/** Getter for CoinsCount, used in USHUDWidget::BindCoinCount() */
	FORCEINLINE uint32 GetCoinCount() const { return CoinCount; }
	
	/** Getter for CoinsCount, used in USHUDWidget::BindLifeCount() */
	FORCEINLINE uint32 GetLifeCount() const { return LifeCount; }
	
	/** Getter for TotalNumWaves, used in USHUDWidget::BindWaveCount() */
	FORCEINLINE uint32 GetTotalNumWaves() const { return TotalNumWaves; }
	
	/** Getter for CurrentWave, used in USHUDWidget::BindWaveCount() */
	FORCEINLINE uint32 GetCurrentWaveNumber() const { return CurrentWaveNumber; }

	/** Setter for TotalNumWaves, called from ASUFOWaveManager::BeginPlay() */
	FORCEINLINE void SetTotalNumWaves(const uint32 Value) { TotalNumWaves = Value; }
	
	/** Setter for CurrentWaveNumber, called from ASUFOWaveManager::SpawnNextWave() */
	FORCEINLINE void SetCurrentWaveNumber(const uint32 Value) { CurrentWaveNumber = Value; }
};
