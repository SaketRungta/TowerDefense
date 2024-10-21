
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STowerSite.generated.h"

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

	UFUNCTION(BlueprintImplementableEvent)
	void SetMaterial(int Index);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> TowerSiteMesh;

	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	TWeakObjectPtr<APawn> PlayerPawn;
};
