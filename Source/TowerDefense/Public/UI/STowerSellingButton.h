
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

public:
	/** Initialize override */
	virtual bool Initialize() override;
	
private:
	/** Button for selling the tower */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SellButton;
	
	/** Text block containing the tower selling price */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> T_TowerSellingPrice;

	/** Sell button clicked callback */
	UFUNCTION()
	void OnSellButtonClicked();
	
	/** Ref to the owning tower as we will have to destroy it when this tower is sold */
	TWeakObjectPtr<ASBaseTower> OwningTower;

	/** Price at which to sell the tower */
	uint32 TowerSellingPrice;
	
public:
	/** Setter for OwningTower, Called from ASBaseTower::BeginPlay */
	FORCEINLINE void SetOwningTower(ASBaseTower* InOwningTower);

	/** Called from ASBaseTower::BeginPlay to the set the selling price of this tower */
	void SetTowerSellingPrice(const uint32& InTowerSellingPrice);

};
