
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

/**
 * Enum for the types of messages to display in HUD
 * Used to set the message type 
 ********************************************************************************************/
UENUM(BlueprintType)
enum class EMessageTypeToDisplay : uint8
{
	Normal,
	Warning,
	Error
};
