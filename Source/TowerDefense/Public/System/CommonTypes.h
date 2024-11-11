
#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.generated.h" 

/**
 * Enum for the types of menu in HUD
 * Used to set the menu to display in the WBP_HUDWidget
 ********************************************************************************************/
UENUM(BlueprintType)
enum class EMenuToShow : uint8
{
	None,
	LevelCompleted,
	TryAgain,
	Pause,
	HUD
};
