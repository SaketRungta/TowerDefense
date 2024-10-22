
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STurretSelectionMenuButton.generated.h"

class UButton;

/**
 * Button for the turret selection menu
 * Turret selection menu houses 4 buttons for the same task so a single class to handle it all
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTurretSelectionMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
private:
	/** Tower selection button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> TurretButton;
};
