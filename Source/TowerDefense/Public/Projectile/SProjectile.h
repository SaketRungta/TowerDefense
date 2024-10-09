
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class ASBaseTower;
class UProjectileMovementComponent;

/**
 * Base projectile class
 * All projectiles derive from this class it is manually activated by tower
 ********************************************************************************************/
UCLASS(Abstract, BlueprintAble, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default Contstructor */
	ASProjectile();

	/** Called when the components have been initialized */
	virtual void PostInitializeComponents() override;

	/** 
	 * Called by the tower which has spwaned this projectile to activate it
	 * This projectile will be a part of the towers object pool system meaning it will deactivated by default
	 * Therefore it will need to be activated by tower when required
	 * @param InFiringSocketTransform
	 */
	void ActivateThisObject(const FTransform& InFiringSocketTransform);

protected:
	/** Root component */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Mesh of the projectile */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	/** Projectile movement component to handle the projectile physics */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

private:
	/** Callback when the projectile mesh overlaps something */
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called by ActivateThisObject via timer so that if the projectile is fired and it does not deactivates by htting a target it should still deactivate after sometime */
	void DeactivateThisObject();

	/** True when in use so that ASBaseTower::FindProjectileFromPool function can find the projectile which is not in use from the object pool */
	bool bIsInUse = false;

	/** Firing socket transform of the owning turret */
	FTransform OwningTurretFiringSocketTransform;

	/** Timer handle which deactivates the turret once activation is called */
	FTimerHandle DeactivateTimer;

public:
	/** Getter for bIsInUse, true when projectile is in use or is active and cannot be used */
	FORCEINLINE const bool IsProjectileInUse() const 
	{ return bIsInUse; }

	/** Time taken by the projectile to deactivate once activated, if the projectile does not hit the target then it will deactivate after given time */
	UPROPERTY(EditAnywhere, Category = ProjectileProperties, meta = (ClampMin = "0.1", ClampMax = "15"))
	float DeactivationTime = 5.f;

	/** Speed at which the projectile can be fired */
	UPROPERTY(EditAnywhere, Category = ProjectileProperties, meta = (ClampMin = "500", ClampMax = "4500"))
	float FiringSpeed = 1500.f;
};
