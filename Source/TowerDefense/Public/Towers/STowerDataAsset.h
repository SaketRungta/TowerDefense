
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "STowerDataAsset.generated.h"

class ASProjectile;

/**
 * Data asset for each tower
 * Stores all the necessary data required by the tower
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType)
class TOWERDEFENSE_API USTowerDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Name of the tower */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	FName TowerName = FName("");
	
	/** Range of the tower */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (ClampMin = "500", ClampMax = "9999"))
	float TowerRange = 550.f;
	
	/** Successive intervals in which tower fires */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (ClampMin = "0.1", ClampMax = "7"))
	float FireRate = 3.f;
	
	/** Price of the tower required to buy it */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerBuyingPrice = 100;
	
	/** Selling price of the tower */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerSellingPrice = 100;

	/** Icon of the tower to show in the tower selection menu */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	FSlateBrush Icon;

	/** The projectile to be fired from this tower */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ASProjectile> ProjectileClass;
	
	/** Count of the projectile that have been spawned for the tower projectile pool */
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectilePoolSize = 5;
	
	/** Base damage dealt by the projectile of this tower */
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (ClampMin = "1", ClampMax = "999"))
	uint32 ProjectileBaseDamage = 1;
	
	/** Sound to play when the tower fires */
	UPROPERTY(EditDefaultsOnly, Category = SFX)
	TObjectPtr<USoundBase> TowerFiringSound;
	
	/** Sound to play when player clicks on this tower */
	UPROPERTY(EditDefaultsOnly, Category = SFX)
	TObjectPtr<USoundBase> TowerClickedSound;

};
