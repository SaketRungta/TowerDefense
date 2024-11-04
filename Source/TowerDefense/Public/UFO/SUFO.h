
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFO.generated.h"

class USplineComponent;
class UWidgetComponent;

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

	/** When UFO is destroyed by the player this delegate will tell the game mode its value to add to coin stash */
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
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Called from the BP when UFO reaches the end of spline path */
	UFUNCTION(BlueprintCallable, Category = UFOData)
	void OnUFOHasReachedBaseCallback();
	
private:
	/** Scene root component */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot = nullptr;

	/** Main mesh of the UFO */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> UFOMesh = nullptr;

	/** Widget to show health bar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> HealthBarWidget = nullptr;

	/** Stores the current health of UFO */
	uint32 CurrentHealth;

	/** Stores the max health of UFO */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "100"))
	uint32 MaxHealth = 10;

	/** Callback when projectile hit the ufo and applies damage */
	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Percentage of health the ufo has 0 is lowest 1 highest */
	float HealthPercentage = 1.f;

	/** Timer to hide the health bar widget once ufo has not been damaged for a long time */
	FTimerHandle HealthBarWidgetVisibilityTimerHandle;
	
	/** Value of the ufo to add when player destroys it */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "100"))
	uint32 UFOValue = 10;

	/** Value of life to deduct when ufo reaches the base */
	UPROPERTY(EditDefaultsOnly, Category = UFOData, meta = (ClampMin = "1", ClampMax = "10"))
	uint32 UFOLifeCount = 1;

public:
	/** Implemented in BP makes the UFO move along the designated spline path */
	UFUNCTION(BlueprintImplementableEvent)
	void MoveAlongSplinePath(const USplineComponent* SplinePath);

	/** Bound to the health bar widget progress bar fill percent, NOTE: Is used in BP */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercentage() const { return HealthPercentage; }

	/** Getter for UFOValue, used to spawn the coins addition widget actor */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetUFOValue() const { return static_cast<int32>(UFOValue); }
	
};
