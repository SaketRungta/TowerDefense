
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SUFO.generated.h"

UCLASS(Abstract, BlueprintType, ClassGroup = Tower)
class TOWERDEFENSE_API ASUFO : public AActor
{
	GENERATED_BODY()
	
public:	
	ASUFO();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* UFOMesh = nullptr;

};
