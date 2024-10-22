// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenu.generated.h"

/**
 * Menu for the user to select tower from
 * This menu pops up when the user clicks on a tower site to select the tower he wishes to spawn
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Called from owning ASTowerSite to play the pop in anim in forward or reverse */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPopInAnimation(bool bIsReversed = false);
};
