
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPlayerPawnInterface.generated.h"

class ASTowerSite;
class ASBaseTower;

UINTERFACE(MinimalAPI)
class USPlayerPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class TOWERDEFENSE_API ISPlayerPawnInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Sets the selected tower site in player pawn class and unselected the last active site if there was any 
	 * Called from ASTowerSite::OnActorClicked
	 */
	virtual void SetCurrentlySelectedTowerSite(ASTowerSite* NewSelectedTowerSite) = 0;
	
	/** 
	 * Sets the selected tower in player pawn class and unselected the last active tower if there was any 
	 * Called from ASBaseTower::OnActorClicked
	 */
	virtual void SetCurrentlySelectedTower(ASBaseTower* NewSelectedTower) = 0;
};
