
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

class USphereComponent;
class ASProjectile;

/**
 * Base class for tower
 * Base class for the child towers to derive from
 * Handles most of the functionality of tower
 * Firing is overloaded in child class
 ********************************************************************************************/
UCLASS(Abstract, BlueprintAble, BlueprintType, ClassGroup = Tower)
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

protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Actor root component */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Base mesh of the tower */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerMesh;

	/** Base mesh of the turret */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> TurretMesh;

	/** Sphere which enforces the range of the turret */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USphereComponent> TurretRangeSphere;

	/** A plane static mesh which has the material that marks the range of the turret */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> TurretRangeIndicatorMesh;

	/** Invoked from ASBaseTower::OnTurretRangeSphereOverlap when enemy is in range to fire the turret */
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
	 * - Becomes the part of unreal's garbage collection system
	 */
	TArray<TWeakObjectPtr<AActor>> InRangeEnemies;

	/** Handles turret firing timers in ASBaseTower::FireTurret and is cleared by ASBaseTower::OnTurretRangeSphereEndOverlap when no of enemies are 0 */
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
	 * - TWeakObjectPtr is not used as we not planning to delete or destroy the projectiles
	 */
	TArray<TObjectPtr<ASProjectile>> ProjectilePool;

private:
	/** Callback when any actor overlaps the TurretRangeSphere */
	UFUNCTION()
	void OnTurretRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Callback when any actor ends overlap with the TurretRangeSphere */
	UFUNCTION()
	void OnTurretRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Called from ASBaseTower::Tick, when any enemy is in range and turret need to point at that enemy */
	void SetTurretLookAtEnemy();

	/** Called from ASBaseTower::BeginPlay to spawn the pool of projectiles for this turret */
	void SpawnProjectilePool();

	/** Number of projectiles this pool can spawn */
	UPROPERTY(EditAnywhere, Category = TowerBehaviour, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectilePoolSize = 5;

public:
	/** Getter for the TurretMesh */
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const 
	{ return TurretMesh; }

};
