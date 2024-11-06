
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenuButton.generated.h"

class UTextBlock;
class UButton;
class UImage;

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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TowerButton;

	/** Tower icon */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TowerIcon;

	/** Tower selling price text block */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> T_TowerPrice;

public:
	/** Getter for TurretButton */
	FORCEINLINE UButton* GetTowerButton() const	{ return TowerButton; }

	/** Called from USTowerSelectionMenu::Initialize to set the tower price */
	void SetTowerPrice(const int32 InTowerPrice) const;

	/** Called from USTowerSelectionMenu::Initialize to set the tower icon */
	void SetTowerIcon(const FSlateBrush& InTowerIcon) const;
	
};
