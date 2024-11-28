
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
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default Constructor */
	ASProjectile();

	/** Called when the components have been initialized */
	virtual void PostInitializeComponents() override;

	/** Begin destroy override, clears all the timers and delegates */
	virtual void BeginDestroy() override;

	/** 
	 * Called by the tower which has spawned this projectile to activate it
	 * This projectile will be a part of the towers object pool system meaning it will be deactivated by default
	 * Therefore it will need to be activated by tower when required
	 * 
	 * @param InFiringSocketTransform socket transform of the turret projectile firing location
	 */
	void ActivateThisObject(const FTransform& InFiringSocketTransform);

protected:
	/** Called whenever this actor is being removed from a level, clears all timers to avoid possible crash */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

#pragma region ComponentsAndCallbacks
	
	/** Root component */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Mesh of the projectile */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	/** Projectile movement component to handle the projectile physics */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** Callback when the projectile mesh overlaps something */
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma endregion ComponentsAndCallbacks
	
	/** Called by ActivateThisObject via timer so that if the projectile is fired, and it does not deactivate by hitting a target it should still deactivate after sometime */
	void DeactivateThisObject();

	/** True when in use so that ASBaseTower::FindProjectileFromPool function can find the projectile which is not in use from the object pool */
	bool bIsInUse = false;

	/** Firing socket transform of the owning turret */
	FTransform OwningTurretFiringSocketTransform;

	/** Timer handle which deactivates the turret once activation is called */
	FTimerHandle DeactivateTimer;

#pragma region ProjectileProperties
	
	/** Damage done to the Ufo when hit by this projectile */
	uint32 BaseDamage = 1.f;

	/** Time taken by the projectile to deactivate once activated, if the projectile does not hit the target then it will deactivate after given time */
	UPROPERTY(EditAnywhere, Category = ProjectileProperties, meta = (ClampMin = "0.1", ClampMax = "15"))
	float DeactivationTime = 5.f;

	/** Speed at which the projectile can be fired */
	UPROPERTY(EditAnywhere, Category = ProjectileProperties, meta = (ClampMin = "500", ClampMax = "99999"))
	float FiringSpeed = 1500.f;
	
	/** True when the projectile is fired from a catapult */
	UPROPERTY(EditDefaultsOnly, Category = ProjectileProperties)
	bool bIsCatapultProjectile = false;

	/** When fired from a catapult the inner radius in which ufo will receive full base damage */
	UPROPERTY(EditDefaultsOnly, Category = Damage, meta = (EditCondition = "bIsCatapultProjectile"))
	float DamageInnerRadius = 250.f;

	/** When fired from a catapult the outer radius in which ufo will receive damage depending on the distance from origin */
	UPROPERTY(EditDefaultsOnly, Category = Damage, meta = (EditCondition = "bIsCatapultProjectile"))
	float DamageOuterRadius = 750.f;

	/** Falloff exponent of damage from DamageInnerRadius to DamageOuterRadius */
	UPROPERTY(EditDefaultsOnly, Category = Damage, meta = (EditCondition = "bIsCatapultProjectile"))
	float DamageFalloff = 0.3f;

#pragma endregion ProjectileProperties
		
	/** Sound when projectile hits a UFO */
	UPROPERTY(EditDefaultsOnly, Category = Default)
	TObjectPtr<USoundBase> ProjectileHitSound;

public:
	/** Getter for bIsInUse, true when projectile is in use or is active and cannot be used */
	FORCEINLINE bool IsProjectileInUse() const
	{ return bIsInUse; }

	/** Setter for BaseDamage, called from ASBaseTower::SpawnProjectilePool */
	FORCEINLINE void SetBaseDamage(const uint32& InBaseDamage)
	{ BaseDamage = InBaseDamage; }
	
};
