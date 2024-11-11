
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/CommonTypes.h"
#include "SHUDWidget.generated.h"

class UTextBlock;
class ASBaseGameMode;
class UWidgetSwitcher;

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
	
	/** Native Destructor override */
	virtual void NativeDestruct() override;

	/** Calls to save the star data for the level */
	UFUNCTION(BlueprintImplementableEvent)
	void SaveLevelStarsData(const int32& LevelIndex, const int32& StarsCount);
	
private:

#pragma	region ComponentsAndCallbacks

	/** Widget switcher to switch between menus */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	/** Text block to show the life count */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_LifeCount;

	/** Text block to show the life count */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_CoinCount;

	/** Text block to show the wave count */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_WaveCount;
	
	/** Binds the life count text block to the game mode */
	UFUNCTION()
	FText BindLifeCount();

	/** Binds the coin count text block to the game mode */
	UFUNCTION()
	FText BindCoinCount() const;

	/** Binds the wave count text block to the game mode */
	UFUNCTION()
	FText BindWaveCount() const;

#pragma	endregion ComponentsAndCallbacks

#pragma region WidgetAnimations
	
	/** Animation to pop out main HUD */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HUDPopOutAnim;
	
	/** Animation to show the level completed menu */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowLevelCompletedMenuAnim;
	
	/** Animation to show try again menu */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowTryAgainMenuAnim;
	
	/** Animation to show paused menu */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowPausedMenuAnim;
	
	/** Animation to show first star popping in */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> PopFirstStarAnim;
	
	/** Animation to show second star popping in */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> PopSecondStarAnim;
	
	/** Animation to show third star popping in */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> PopThirdStarAnim;

	/** Pop out the main HUD widget */
	void PopOutHUD(const int32& InWidgetIndex, UWidgetAnimation* InAnimToPlay);

	/** Pops in the main HUD widget */
	void PopInHUD();

	/**
	 * Called when pop out HUD anim finishes playing
	 * Plays the anim to show the requested menu
	 */
	UFUNCTION()
	void ShowTheRequestedMenu();

	/**
	 * Show the main HUD of this class
	 * Called from PopInHUD when the last played anim finishes playing in reverse
	 * So that the menu that was last opened is now gone
	 * Now we need to show the main HUD
	 */
	UFUNCTION()
	void ShowTheMainHUD();

	/**
	 * Called from ShowTheRequestedMenu, when level completed menu is popped up
	 * To show the stars rewarded to the user by playing the popping in animation one by one
	 */
	UFUNCTION()
	void PlayStarsAnim();

	/**
	 * Animation which was last played other than the hud pop in anim
	 * When we play the level finished anim we need to call to play the stars popping in anim
	 * When game is paused and we unpause it we need to play the pause menu in reverse
	 * It helps us to add modularity to our code instead of hard coding it
	 */
	TWeakObjectPtr<UWidgetAnimation> LastPlayedAnim;

	/** Index of the widget we need to activate as all the menus are in their separate panels */
	int32 WidgetIndexToActivate = 0;

	/** Amount of stars awarded to the player for this level */
	int32 StarsAwarded = 1;

	/** Stores the count of stars popping anim that have been played so that we do not play more than required */
	int32 StarPopInAnimPlayedCount = 0;

	/** Delegate to call USHUDWidget::PlayStarsAnim once a star anim finishes playing */
	FWidgetAnimationDynamicEvent FinishedPlayingStarAnim;
	
#pragma endregion WidgetAnimations

	/** Checks the level name and calls to save the game data for the level */
	void SetLevelStarsData();

	/** Ref to the game mode as we will be binding all the data from game mode */
	TWeakObjectPtr<ASBaseGameMode> GameMode;

	/** Stores the initial life count as our bp will need it show the stars once level is completed */
	uint32 InitialLifeCount = 0;
	
	/** Stores the current life count as our bp will need it show the stars once level is completed */
	uint32 CurrentLifeCount = 0;

public:
	/** Called from base HUD to show error messages on the screen */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowErrorMessage(const FString& InErrorMessage);

	/** Called from base HUD to show the requested menu */
	void ShowTheGivenMenu(const EMenuToShow InMenuToShow);

};
