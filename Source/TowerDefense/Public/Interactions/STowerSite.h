
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STowerSite.generated.h"

class UWidgetComponent;

UCLASS(BlueprintAble, BlueprintType, ClassGroup = Interactions)
class TOWERDEFENSE_API ASTowerSite : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTowerSite();

	virtual void PostInitializeComponents() override;

	void DeactivateTowerSite();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerSiteMesh;

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	TWeakObjectPtr<APawn> PlayerPawn;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerSiteMesh() const { return TowerSiteMesh; }
};
