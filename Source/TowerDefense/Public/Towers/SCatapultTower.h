
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

	/** Called once all the components of the actor has been initialized */
	virtual void PostInitializeComponents() override;

	/** Called from parent class to show the inner range sphere */
	void ShowInnerRangeIndicatorMesh(const bool bShowSphere = true) const;
	
private:
	/** Mesh for the turret base */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TurretBase;

	/** Mesh for fake catapult projectile */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> CatapultProjectileMesh;

	/** Sphere which enforces the inner range of the turret */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USphereComponent> TowerInnerRangeSphere;

	/** A plane static mesh which has the material that marks the inner range of the turret */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerInnerRangeIndicatorMesh;

	/** Callback when any actor overlaps the TowerInnerRangeSphere */
	UFUNCTION()
	void OnTowerInnerRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Callback when any actor ends overlap with the TowerInnerRangeSphere */
	UFUNCTION()
	void OnTowerInnerRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Sets the pitch of the turret mesh when turret is firing, called by BP, majorly for cosmetic purpose */
	UFUNCTION(BlueprintCallable)
	void SetTurretMeshPitch(const float& InPitch);

	/**
	 * Called from the blueprint when turret reaches the end of timeline and now the projectile has to be fired
	 * Hides the CatapultProjectileMesh and activates a projectile from the pool
	 */
	UFUNCTION(BlueprintCallable)
	void FireProjectile();

	/** Inner range of the tower in which catapult does not functions */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	float TowerInnerRange = 500.f;
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

	/** Invoked from ASBaseTower::OnTowerRangeSphereOverlap when enemy is in range to fire the turret */
	virtual bool FireTurret() override;

	/** Implemented in BP this class fires a timeline which rotates the catapult and then retreats, it also calls when to release the projectile */
	UFUNCTION(BlueprintImplementableEvent)
	void FireCatapult();

public:
	/** Getter for CatapultProjectileMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTurretBase() const 
	{ return TurretBase; }
	
	/** Getter for CatapultProjectileMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetCatapultProjectileMesh() const 
	{ return CatapultProjectileMesh; }
	
};
