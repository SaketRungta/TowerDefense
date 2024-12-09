
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRandomPrefabTile.generated.h"

UCLASS()
class TOWERDEFENSE_API ASRandomPrefabTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASRandomPrefabTile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetRandomPrefabTile();
	
private:
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> PrefabMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	TArray<TObjectPtr<UStaticMesh>> PrefabMeshes;
	
};
