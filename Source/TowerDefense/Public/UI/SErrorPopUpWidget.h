
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SErrorPopUpWidget.generated.h"

/**
 * Error pop up widget
 * Widget to show any error messages
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USErrorPopUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
};
