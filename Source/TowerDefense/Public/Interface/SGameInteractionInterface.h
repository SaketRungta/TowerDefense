
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "System/CommonTypes.h"
#include "SGameInteractionInterface.generated.h"

class ASTowerSite;
class ASBaseTower;

UINTERFACE(MinimalAPI)
class USGameInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class TOWERDEFENSE_API ISGameInteractionInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Sets the selected tower site in player pawn class and unselected the last active site if there was any 
	 * Called from ASTowerSite::OnActorClicked
	 */
	virtual void SetCurrentlySelectedTowerSite(ASTowerSite* NewSelectedTowerSite) { }
	
	/** 
	 * Sets the selected tower in player pawn class and unselected the last active tower if there was any 
	 * Called from ASBaseTower::OnActorClicked
	 */
	virtual void SetCurrentlySelectedTower(ASBaseTower* NewSelectedTower) { }
	
	/**
	 * Shows any error message on the screen
	 * Called from turret selection menu to show not enough coins error message
	 */
	virtual void ShowErrorMessage(const FString& InErrorMessage) { }

	/**
	 * Called from player pawn to show the pause menu
	 * Game mode to show the try again menu
	 * Wave manager to show the level completed menu
	 */
	virtual void ShowTheGivenMenu(EMenuToShow InMenuToShow) { }

	/**
	 * Called from game mode and wave manager
	 * Sets the bool value of bCanPauseTheGame in player pawn class
	 * So once the game is over player cannot pause it
	 */
	virtual void SetCanPauseTheGame(const bool InCanPauseGame) { }
};
