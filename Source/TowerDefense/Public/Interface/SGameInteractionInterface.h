
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
	
};
