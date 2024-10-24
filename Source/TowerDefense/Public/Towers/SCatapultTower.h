
#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SCatapultTower.generated.h"

/**
 * Catapult tower
 * Fires single shot dealing huge damage in area
 * Slow but powerful deals
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASCatapultTower : public ASBaseTower
{
	GENERATED_BODY()
	
public:
	/** Default constructor */
	ASCatapultTower();

	/** Tick event override */
	virtual void Tick(float DeltaTime) override;

private:
	/** Mesh for the turret base */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TurretBase;

	/** Mesh for fake catapult projectile */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> CatapultProjectileMesh;

	/** Sets the pitch of the turret mesh when turret is firing, called by BP, majorly for cosmetic purpose */
	UFUNCTION(BlueprintCallable)
	void SetTurretMeshPitch(const float& InPitch);

	/**
	 * Called from the blueprint when turret reaches the end of timeline and now the projectile has to be fired
	 * Hides the CatapultProjectileMesh and activates a projectile from the pool
	 */
	UFUNCTION(BlueprintCallable)
	void FireProjectile();

protected:
	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual void FireTurret() override;

	/** Implemented in BP this class fires a timeline which rotates the catapult and then retreats, it also calls when to release the projectile */
	UFUNCTION(BlueprintImplementableEvent)
	void FireCatapult();

public:
	/** Getter for CatapultProjectileMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetCatapultProjectileMesh() const 
	{ return CatapultProjectileMesh; }
};
