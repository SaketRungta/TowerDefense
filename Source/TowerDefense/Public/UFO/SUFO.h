
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFO.generated.h"

class USplineComponent;
class UWidgetComponent;
class ASUFOWaveManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideHealthWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetIfAnimIsPlaying);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUFODestroyed, int32, UFOValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUFOReachedBase, uint32, UFOLifeCount);

/**
 * Base UFO class
 * UFO for the towers to shoot at
 ********************************************************************************************/
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UFO)
class TOWERDEFENSE_API ASUFO : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASUFO();

#pragma region Delegates
	
	/**
	 * When UFO is destroyed by the player this delegate will tell the game mode its value to add to coin stash
	 * In the blueprints it spawns the coin reward widget actor passing the value of this ufo
	 */
	UPROPERTY(BlueprintAssignable, Category = UFOData)
	FOnUFODestroyed OnUFODestroyed;
	
	/** When UFO reaches end this delegate will tell the game mode to deduct a life */
	FOnUFOReachedBase OnUFOReachedBase;

	/**
	 * Called once the timer for damage runs out and now health bar widget can be hidden
	 * Bound by health bar widget in the bp to play the fading anim
	 */
	UPROPERTY(BlueprintAssignable, Category = UFOData)
	FOnHideHealthWidget OnHideHealthWidget;

	/**
	 * Called when ufo takes any damage
	 * Bound by the health bar widget in the bp to stop health bar anim from playing and setting its opacity back to full
	 */
	UPROPERTY(BlueprintAssignable, Category = UFOData)
	FOnResetIfAnimIsPlaying OnResetIfAnimIsPlaying;

#pragma endregion Delegates
	
	/**
	 * Called from ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath when ufo reaches the end of its spline path
	 * Broadcasts OnUFOReachedBase delegate to deduct life from game mode and destroys the ufo
	 */
	void OnUFOReachedBaseCall();
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

private:

#pragma region Components
	
	/** Scene root component */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot = nullptr;

	/** Main mesh of the UFO */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> UFOMesh = nullptr;

	/** Widget to show health bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HealthBarWidget = nullptr;

#pragma endregion Components
	
	/** Callback when projectile hit the ufo and applies damage */
	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Timer to hide the health bar widget once ufo has not been damaged for a long time */
	FTimerHandle HealthBarWidgetVisibilityTimerHandle;

#pragma region UFOData
	
	/** Stores the current health of UFO */
	uint32 CurrentHealth;

	/** Stores the max health of UFO */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "100"))
	uint32 MaxHealth = 10;

	/** Percentage of health the ufo has 0 is lowest 1 highest */
	float HealthPercentage = 1.f;

	/** Coin value of the ufo to add when player destroys it */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "100"))
	uint32 UFOCoinValue = 10;

	/** Value of life to deduct when ufo reaches the base */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "10"))
	uint32 UFOLifeDamageCount = 1;

#pragma endregion UFOData
	
	/** Spline for the ufo to follow, set from ASUFOWaveManager::SpawnUFOs via setter */
	TObjectPtr<USplineComponent> SplinePath;

	/** Stores the distance ufo has travelled along its spline path */
	float DistanceAlongSpline = 0.0f;

	/** Speed at which ufo moves along the spline path */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "0.1", ClampMax = "5000"))
	float MovementSpeed = 300.f;

	/**
	 * Ref to the wave manager class, set via the setter
	 * When UFO is destroyed by the player then we need to tell the wave manager class
	 * So that wave manager removes it from the list of UFOs to move
	 */
	TObjectPtr<ASUFOWaveManager> WaveManager;
	
public:
	/** Bound to the health bar widget progress bar fill percent, NOTE: Is being used in BP */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() const
	{ return HealthPercentage; }

	/** Setter for SplinePath, called from ASUFOWaveManager::SpawnUFOs */
	FORCEINLINE void SetSplinePath(USplineComponent* InSplinePath)
	{ SplinePath = InSplinePath; }

	/** Getter for SplinePath, called from ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath */
	FORCEINLINE const USplineComponent* GetSplinePath() const
	{ return SplinePath; }

	/** Getter for DistanceAlongSpline, called from ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath */
	FORCEINLINE float GetDistanceAlongSpline() const
	{ return DistanceAlongSpline; }
	
	/** Setter for DistanceAlongSpline, called from ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath */
	FORCEINLINE void SetDistanceAlongSpline(const float& InDistance)
	{ DistanceAlongSpline = InDistance; }
	
	/** Getter for MovementSpeed, called from ASUFOWaveManager::MoveAllUFOsAlongTheSplinePath */
	FORCEINLINE float GetMovementSpeed() const
	{ return MovementSpeed; }
	
	/** Setter for WaveManager, called from ASUFOWaveManager::SpawnUFOs */
	FORCEINLINE void SetWaveManager(ASUFOWaveManager* InWaveManager)
	{ WaveManager = InWaveManager; }
	
};
