
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SBaseGameMode.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = GameMode)
class TOWERDEFENSE_API ASBaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	uint32 CoinsCount = 10;
	uint32 LifeCount = 10;

public:
	FORCEINLINE uint32 GetCoinsCount() const { return CoinsCount; }
	FORCEINLINE uint32 GetLifeCount() const { return LifeCount; }

	
};
