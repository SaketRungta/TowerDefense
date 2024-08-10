
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseTower.generated.h"

UCLASS()
class TOWERDEFENSE_API ASBaseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	ASBaseTower();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Components)
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* TowerMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* Turret = nullptr;

private:

};
