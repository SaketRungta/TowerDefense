
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

class USphereComponent;
class ASProjectile;
class UWidgetComponent;
class ASTowerSite;
class ASTowerDefensePawn;

/**
 * Struct for the tower data table
 * Stores all the necessary data required by the tower
 * 
 * @param ProjectileClass: The projectile to be fired from this tower
 * @param FireRate: Successive intervals in which tower fires
 * @param ProjectilePoolSize: Count of the projectile that have been spawned for the tower projectile pool
 * @param ProjectileBaseDamage: Base damage dealt by the projectile of this tower
 * @param TowerBuyingPrice: Price of the tower required to buy it
 * @param TowerSellingPrice: Selling price of the tower
 * @param Icon: Icon of the tower to show in the tower selection menu 
 ********************************************************************************************/
USTRUCT(BlueprintType)
struct FTowerDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1", ClampMax = "7"))
	float FireRate = 3.f;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectilePoolSize = 5;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectileBaseDamage = 1;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerBuyingPrice = 100;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerSellingPrice = 100;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush Icon;
	
};

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

	/** Begin destroy override */
	virtual void Destroyed() override;

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	/** Called once all the components of the actor has been initialized */
	virtual void PostInitializeComponents() override;

	/** Called from Player Pawn when RMB/LMB is clicked to set tower to unselected */
	void SetTowerToUnselected();

	/** Called from child classes to initialize tower data from data table */
	void UpdateTowerDataFromDataTable(const FName InTowerName);
	
protected:
	/** Begin play overloading */
	virtual void BeginPlay() override;

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
	
	/** The projectile that has to be spawned by this class */
	UPROPERTY(VisibleAnywhere, Category = TowerData)
	TSubclassOf<ASProjectile> ProjectileClass;
	
	/** Intervals of time in which this turret can fire */
	UPROPERTY(VisibleAnywhere, Category = TowerData)
	float FireRate = 3.f;

	/** Number of projectiles this pool can spawn */
	UPROPERTY(VisibleAnywhere, Category = TowerData)
	uint32 ProjectilePoolSize = 5;
	
	/** Base damage dealt by the projectile of this tower */
	UPROPERTY(VisibleAnywhere, Category = TowerData)
	uint32 ProjectileBaseDamage = 1;
	
	/** Selling price of the tower */
	UPROPERTY(VisibleAnywhere, Category = TowerData)
	uint32 TowerSellingPrice = 100;

#pragma endregion TowerData;
	
	/** True when tower should fire, set from range sphere pverlap callbacks */
	bool bShouldFire = false;
	
	/** True when tower is selected */
	bool bIsTowerSelected = false;
	
	/** Ref to the player pawn so that we call set the last selected tower via interface */
	TObjectPtr<ASTowerDefensePawn> PlayerPawn;

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
	TObjectPtr<ASTowerSite> TowerSite;
	
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
	FORCEINLINE void SetTowerSite(ASTowerSite* InTowerSite)
	{ TowerSite = InTowerSite; }
	
};
