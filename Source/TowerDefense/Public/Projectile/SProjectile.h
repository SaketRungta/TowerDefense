
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class UProjectileMovementComponent;
class ASBaseTower;

UCLASS(Abstract, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

	virtual void PostInitializeComponents() override;

	void ActivateThisObject(const FTransform& InTurretSocketTransform);

protected:
	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

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
