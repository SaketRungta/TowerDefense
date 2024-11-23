
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "System/CommonTypes.h"

#include "SBaseHUD.generated.h"

class USHUDWidget;

/**
 * HUD class for the game
 * Renders the main HUD to the screen
 * Handles UI interaction with other classes
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, Category = UI)
class TOWERDEFENSE_API ASBaseHUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** Begin play override */
	virtual void BeginPlay() override;

private:
	/** Main HUD widget class to be displayed on the viewport */
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<USHUDWidget> HUDWidgetClass;

	/** Ref to the main HUD widget */
	TWeakObjectPtr<USHUDWidget> HUDWidget;
	
public:
	/** Show's the in error messages on the viewport */
	void ShowMessageInHUD(const FString& InMessage, EMessageTypeToDisplay InMessageType) const;
	
	/** Show's the menu as per request */
	void ShowTheGivenMenu(const EMenuToShow InMenuToShow) const;
	
};
