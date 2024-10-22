
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STurretSelectionMenu.generated.h"

/**
 * Menu for the user to select tower from
 * This menu pops up when the user clicks on a tower site to select the tower he wishes to spawn
 ********************************************************************************************/
UCLASS()
class TOWERDEFENSE_API USTurretSelectionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Called from owning ASTowerSite to play the pop in anim in forward or reverse */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPopInAnimation(bool bIsReversed = false);
};
