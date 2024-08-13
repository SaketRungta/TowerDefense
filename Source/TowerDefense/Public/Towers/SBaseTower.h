
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

class USphereComponent;
class ASProjectile;
class UArrowComponent;

UCLASS(Abstract, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASBaseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	ASBaseTower();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* TowerMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* Turret = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	USphereComponent* TurretRangeSphere = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* TurretRangeIndicatorMesh = nullptr;

	virtual void FireTurret() PURE_VIRTUAL(ASBaseTower::FireTurret);

	ASProjectile* FindProjectileFromPool();

	TArray<AActor*> InRangeEnemies = {};

	FTimerHandle FireCooldownTimer;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<ASProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = TowerBehaviour)
	float FireRate = 3.f;

	TArray<ASProjectile*> ProjectilePool = {};

private:

	UFUNCTION()
	void OnInTurretRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnInTurretRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetTurretLookAtEnemy();

	void SpawnProjectilePool();

	uint32 ProjectilePoolSize = 5;

	/*
public:

	FORCEINLINE const UStaticMeshComponent* GetTurret() const { return Turret; }
	*/
};
