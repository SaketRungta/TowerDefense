
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenuButton.generated.h"

class UButton;

/**
 * Button for the tower selection menu
 * Tower selection menu houses 4 buttons for the same task so a single class to handle it all
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSelectionMenuButton : public UUserWidget
{
	GENERATED_BODY()

private:
	/** Tower selection button */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> TowerButton;

	/** Stores the price of the tower, set from WBP_TowerSelectionMenu */
	int32 TowerPrice = 100;

public:
	/** Getter for TurretButton */
	FORCEINLINE UButton* GetTowerButton() const	{ return TowerButton; }

	/** Setter for TowerPrice, set from tower selection menu */
	FORCEINLINE void SetTowerPrice(int32 InTowerPrice) { TowerPrice = InTowerPrice; }
	
	/** Getter for TowerPrice */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetTowerPrice() const	{ return TowerPrice; }
	
};
