
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

class USphereComponent;
class ASProjectile;
class UWidgetComponent;
class ASTowerSite;
class ASTowerDefensePawn;
class USTowerDataAsset;

/**
 * Base class for tower
 * Base class for the child towers to derive from
 * Handles most of the functionality of tower
 * Firing is overloaded in child class
 ********************************************************************************************/
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASBaseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASBaseTower();
	
	/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
	virtual void Destroyed() override;

	/** Begin destroy override, clears all the timers and delegates */
	virtual void BeginDestroy() override;

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	/** Called once all the components of the actor has been initialized */
	virtual void PostInitializeComponents() override;

	/** Called from Player Pawn when RMB/LMB is clicked to set tower to unselected */
	void SetTowerToUnselected();

protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Called whenever this actor is being removed from a level, clears all timers to avoid possible crash */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual bool FireTurret();

	/** Sets the emissive value of materials when hovered/unhovered by the user */
	UFUNCTION(BlueprintNativeEvent)
	void SetTowerEmissiveValue(const float EmissiveValue = 0.f);
	
	/** 
	 * Returns true if there is any projectile in the pool, called from derived classes
	 * @param InProjectileRef reference to the projectile variable that is passed along to be initialized by the first projectile that is not in use
	 */
	bool FindProjectileFromPool(ASProjectile*& InProjectileRef);

private:

#pragma region ComponentsAndCallback
	
	/** Actor root component */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Base mesh of the tower */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerMesh;

	/** Base mesh of the turret */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TurretMesh;

	/** Sphere which enforces the range of the turret */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USphereComponent> TowerRangeSphere;

	/** A plane static mesh which has the material that marks the range of the turret */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerRangeIndicatorMesh;

	/** Widget to sell tower */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UWidgetComponent> TowerSellingWidget;

	/** Callback when tower is clicked, sets the range mesh and selling widget to visible */
	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	/** Callback when any actor overlaps the TowerRangeSphere */
	UFUNCTION()
	void OnTowerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Callback when any actor ends overlap with the TowerRangeSphere */
	UFUNCTION()
	void OnTowerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion ComponentsAndCallback
	
	/** Called from ASBaseTower::Tick, when any enemy is in range and turret need to point at that enemy */
	void SetTurretLookAtEnemy();

	/** Called from ASBaseTower::BeginPlay to spawn the pool of projectiles for this turret */
	void SpawnProjectilePool();

#pragma region TowerData;
	
	/** The data asset class for this tower */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TObjectPtr<USTowerDataAsset> TowerDataAsset;
	
	/** The projectile that has to be spawned by this class */
	TSubclassOf<ASProjectile> ProjectileClass;
	
	/** Intervals of time in which this turret can fire */
	float FireRate = 3.f;

	/** Number of projectiles this pool can spawn */
	uint32 ProjectilePoolSize = 5;
	
	/** Base damage dealt by the projectile of this tower */
	uint32 ProjectileBaseDamage = 1;
	
	/** Selling price of the tower */
	uint32 TowerSellingPrice = 100;

	/** Sound to play when the tower fires */
	UPROPERTY()
	TObjectPtr<USoundBase> TowerFiringSound;
	
#pragma endregion TowerData;
	
	/** True when tower should fire, set from range sphere pverlap callbacks */
	bool bShouldFire = false;
	
	/** True when tower is selected */
	bool bIsTowerSelected = false;
	
	/** Ref to the player pawn so that we call set the last selected tower via interface */
	TWeakObjectPtr<ASTowerDefensePawn> PlayerPawn;

	/** Contains all the enemies currently in range of the turret */
	TArray<TWeakObjectPtr<AActor>> InRangeEnemies;

	/** Handles turret firing timers in ASBaseTower::FireTurret and is cleared by ASBaseTower::OnTowerRangeSphereEndOverlap when no of enemies are 0 */
	FTimerHandle FireCooldownTimer;

	/** Object pool that contains all the projectiles this turret has */
	TArray<TObjectPtr<ASProjectile>> ProjectilePool;

	/**
	 * Ref to the tower site on which this tower sits
	 * When this tower is sold by the user we need to enable the tower
	 */
	TWeakObjectPtr<ASTowerSite> TowerSite;
	
public:
	/** Getter for the TurretMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerMesh() const
	{ return TowerMesh; }
	
	/** Getter for the TurretMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const
	{ return TurretMesh; }

	/** Setter for TowerSite, called from USTowerSelectionMenu::SpawnGivenTower */
	FORCEINLINE void SetTowerSite(ASTowerSite* InTowerSite);
	
};
