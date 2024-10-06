
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

UCLASS(Abstract, BlueprintAble, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

	virtual void PostInitializeComponents() override;

	void ActivateThisObject(const class ASBaseTower* OwningTower);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DeactivateThisObject();

	bool bIsInUse = false;

	FTransform TurretSocketTransform;

	FTimerHandle DeactivateTimer;

public:
	const bool IsProjectileInUse() const { return bIsInUse; }

};
