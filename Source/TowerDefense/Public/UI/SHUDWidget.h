
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHUDWidget.generated.h"

class UTextBlock;
class ASBaseGameMode;

/**
 * Main HUD of the game
 * Displays all the necessary data required like coins, lives and waves count
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Used to initialize widget components with bindings */
	virtual bool Initialize() override;

protected:
	/** Native Constructor override */
	virtual void NativeConstruct() override;
	
private:
	/** Text block to show the life count */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> LifeCount;

	/** Text block to show the life count */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CoinCount;

	/** Text block to show the wave count */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> WaveCount;

	/** Ref to the game mode as we will be binding all the data from game mode */
	TWeakObjectPtr<ASBaseGameMode> GameMode;

	/** Binds the life count text block to the game mode */
	UFUNCTION()
	FText BindLifeCount() const;

	/** Binds the coin count text block to the game mode */
	UFUNCTION()
	FText BindCoinCount() const;

	/** Binds the wave count text block to the game mode */
	UFUNCTION()
	FText BindWaveCount() const;

public:
	/** Called from base HUD to show error messages on the screen */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowErrorMessage(const FString& InErrorMessage);

	/** Called from base HUD to show the requested menu */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowTheGivenMenu(EMenuToShow InMenuToShow);
};
