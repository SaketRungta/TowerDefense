
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

class USphereComponent;
class ASProjectile;
class UWidgetComponent;
class ISGameInteractionInterface;

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

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	/** Called once all the components of the actor has been initialized */
	virtual void PostInitializeComponents() override;

	/** Called from Player Pawn when RMB/LMB is clicked to set tower to unselected */
	void SetTowerToUnselected();
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret();

	/** 
	 * Returns true if there is any projectile in the pool
	 * 
	 * @param InProjectileRef reference to the projectile variable that is passed along to be initialized by the first projectile that is not in use
	 */
	bool FindProjectileFromPool(ASProjectile*& InProjectileRef);

	/** 
	 * Contains all the enemies currently in range of the turret 
	 * 
	 * Notes
	 * - TweakObjectPtr as we are dynamically deleting the enemies so it will automatically set it to nullptr
	 * - Validity can be checked at any moment
	 * - Becomes the part of unreal garbage collection system
	 */
	TArray<TWeakObjectPtr<AActor>> InRangeEnemies;

	/** Handles turret firing timers in ASBaseTower::FireTurret and is cleared by ASBaseTower::OnTowerRangeSphereEndOverlap when no of enemies are 0 */
	FTimerHandle FireCooldownTimer;

	/** The projectile that has to be spawned by this class */
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<ASProjectile> ProjectileClass;

	/** Intervals of time in which this turret can fire */
	UPROPERTY(EditAnywhere, Category = TowerBehaviour, meta = (ClampMin = "0.1", ClampMax = "7"))
	float FireRate = 3.f;

	/** 
	 * Object pool that contains all the projectiles this turret has
	 * 
	 * Notes
	 * - TObjectPtr will help in garbage collection as this in not associated with UPROPERTY()
	 * - Efficient and safer alternative to raw pointers when dealing with UObjects
	 * - TWeakObjectPtr is not used as we're not planning to delete or destroy the projectiles
	 */
	TArray<TObjectPtr<ASProjectile>> ProjectilePool;

	/** Sets the emissive value of materials when hovered/unhovered by the user */
	UFUNCTION(BlueprintImplementableEvent)
	void SetTowerEmissiveValue(const float EmissiveValue = 0.f);
	
private:
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

	/** True when tower is selected */
	bool bIsTowerSelected = false;
	
	/** Callback when any actor overlaps the TowerRangeSphere */
	UFUNCTION()
	void OnTowerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Callback when any actor ends overlap with the TowerRangeSphere */
	UFUNCTION()
	void OnTowerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Called from ASBaseTower::Tick, when any enemy is in range and turret need to point at that enemy */
	void SetTurretLookAtEnemy();

	/** Called from ASBaseTower::BeginPlay to spawn the pool of projectiles for this turret */
	void SpawnProjectilePool();

	/** Number of projectiles this pool can spawn */
	UPROPERTY(EditAnywhere, Category = TowerBehaviour, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectilePoolSize = 5;

	/** Ref to the player pawn so that we call set the last selected tower via interface */
	TWeakObjectPtr<APawn> PlayerPawn;

	/** Interface to the player pawn to set the last selected tower */
	ISGameInteractionInterface* GameInteractionInterface;

public:
	/** Getter for the TurretMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerMesh() const
	{ return TowerMesh; }
	
	/** Getter for the TurretMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const 
	{ return TurretMesh; }
};
