
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
	virtual void SetCurrentlyActiveTowerSite(ASTowerSite* CurrentlyActiveTowerSite) = 0;
};
