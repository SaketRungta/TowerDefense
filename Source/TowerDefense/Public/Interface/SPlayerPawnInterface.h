
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPlayerPawnInterface.generated.h"

class ASTowerSite;

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
	 * Sets the currenlty active tower site in player pawn class and deactivates the last active site if there was any 
	 * Called from ASTowerSite::OnActorClicked
	 */
	virtual void SetCurrentlyActiveTowerSite(ASTowerSite* CurrentlyActiveTowerSite) = 0;
};
