
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
	UPROPERTY(EditDefaultsOnly)
	FName TowerName = FName("");
	
	/** The projectile to be fired from this tower */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASProjectile> ProjectileClass;
	
	/** Successive intervals in which tower fires */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1", ClampMax = "7"))
	float FireRate = 3.f;
	
	/** Count of the projectile that have been spawned for the tower projectile pool */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectilePoolSize = 5;
	
	/** Base damage dealt by the projectile of this tower */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "15"))
	uint32 ProjectileBaseDamage = 1;
	
	/** Price of the tower required to buy it */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerBuyingPrice = 100;
	
	/** Selling price of the tower */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "50", ClampMax = "500"))
	uint32 TowerSellingPrice = 100;

	/** Icon of the tower to show in the tower selection menu */
	UPROPERTY(EditDefaultsOnly)
	FSlateBrush Icon;

	/** Sound to play when the tower fires */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> TowerFiringSound;
	
};
