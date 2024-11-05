
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSellingButton.generated.h"

class UButton;
class UTextBlock;
class ASBaseTower;

/**
 * Button for the selling the tower
 * Widget that handles the tower selling functionality
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSellingButton : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SellButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> T_TowerSellingPrice;

	int32 TowerSellingPrice;

	TWeakObjectPtr<ASBaseTower> OwningTower;
	
public:
	FORCEINLINE void SetOwningTower(ASBaseTower* InOwningTower);
	
	FORCEINLINE void SetTowerSellingPrice(const int32& InTowerSellingPrice) { TowerSellingPrice = InTowerSellingPrice; }

};
