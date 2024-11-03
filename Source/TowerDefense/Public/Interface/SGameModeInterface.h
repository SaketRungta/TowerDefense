
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameModeInterface.generated.h"

UINTERFACE(MinimalAPI)
class USGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class TOWERDEFENSE_API ISGameModeInterface
{
	GENERATED_BODY()

public:
	virtual void AddCoins(const uint32 CoinsToAdd) = 0;
	virtual void DeductLife() = 0;
	
};
