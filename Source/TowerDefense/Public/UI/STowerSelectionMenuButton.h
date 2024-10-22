
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenuButton.generated.h"

class UButton;

/**
 * Button for the tower selection menu
 * Tower selection menu houses 4 buttons for the same task so a single class to handle it all
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSelectionMenuButton : public UUserWidget
{
	GENERATED_BODY()

private:
	/** Tower selection button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> TurretButton;
};
