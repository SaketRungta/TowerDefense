
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/SGameInteractionInterface.h"
#include "SBaseHUD.generated.h"

class USHUDWidget;

/**
 * HUD class for the game
 * Renders the main HUD to the screen
 * Handles UI interaction with other classes
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, Category = UI)
class TOWERDEFENSE_API ASBaseHUD : public AHUD, public ISGameInteractionInterface
{
	GENERATED_BODY()

public:
	/** Interface override used to show error messages on screen */
	virtual void ShowErrorMessage(const FString& InErrorMessage) override;
	
	/** Interface override used to show menu as per request */
	virtual void ShowTheGivenMenu(EMenuToShow InMenuToShow) override;
	
protected:
	/** Begin play override */
	virtual void BeginPlay() override;

private:
	/** Main HUD widget class to be displayed on the viewport */
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<USHUDWidget> HUDWidgetClass;

	/** Ref to the main HUD widget */
	TWeakObjectPtr<USHUDWidget> HUDWidget;
	
};
