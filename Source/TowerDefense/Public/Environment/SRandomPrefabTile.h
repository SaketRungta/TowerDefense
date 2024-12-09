
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRandomPrefabTile.generated.h"

/**
 * Random prefab tile to place in the level
 * Purely for cosmetic purpose this class selects a random prefab from an array
 * Then generates a seed that is truly random and fetches a prefab from the array
 ********************************************************************************************/
UCLASS()
class TOWERDEFENSE_API ASRandomPrefabTile : public AActor
{
	GENERATED_BODY()
	
public:
	/** Default Constructor */
	ASRandomPrefabTile();

protected:
	/** Begin play override */
	virtual void BeginPlay() override;

	/**
	 * Generates a random seed for each instance of the actor and gets a truly random integer
	 * This integer is then used to get the prefab static mesh for this actor
	 * Also rotates the actor to a random angle that is multiple of 90
	 *
	 * Blueprint callable as the blueprint construction script calls this function to update the prefab
	 * When we make any changes C++ one functions in a different way
	 */
	UFUNCTION(BlueprintCallable)
	void SetRandomPrefabTile();
	
private:
	/** Static mesh component for the prefab */
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> PrefabMeshComponent;

	/** Array to store all of the prefab static meshes */
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	TArray<TObjectPtr<UStaticMesh>> PrefabMeshes;
	
};
