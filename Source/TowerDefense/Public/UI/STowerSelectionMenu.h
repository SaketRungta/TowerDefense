
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenu.generated.h"

class USTowerSelectionMenuButton;
class ASTowerSite;
class ASBaseTower;
class ASTowerDefensePawn;
class ASBaseGameMode;
class ASBaseHUD;
class USTowerDataAsset;

/**
 * Menu for the user to select tower from
 * This menu pops up when the user clicks on a tower site to select the tower he wishes to spawn
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Used to initialize widget components with bindings */
	virtual bool Initialize() override;

protected:
	/** Native destruct override */
	virtual void NativeDestruct() override;
	
private:

#pragma region UIMethodsAndCallbacks
	
	/** Canon tower selection button */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USTowerSelectionMenuButton> CannonButton;

	/** Machine gun tower selection button */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USTowerSelectionMenuButton> MachineGunButton;

	/** Archer tower selection button */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USTowerSelectionMenuButton> ArcherTowerButton;

	/** Catapult tower selection button */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USTowerSelectionMenuButton> CatapultButton;

	/** Menu popping in animation */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> PopInAnimation;
	
	/** Callback when user clicks on the canon tower button */
	UFUNCTION()
	void OnCannonButtonClicked();

	/** Callback when user clicks on the machine gun tower button */
	UFUNCTION()
	void OnMachineGunButtonClicked();

	/** Callback when user clicks on the archer tower button */
	UFUNCTION()
	void OnArcherTowerButtonClicked();

	/** Callback when user clicks on the catapult tower button */
	UFUNCTION()
	void OnCatapultButtonClicked();

#pragma endregion UIMethodsAndCallbacks
	
#pragma region TowerData

	/** Array to store the data asset of all towers, used to fill in the price and icon for each tower */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TArray<TObjectPtr<USTowerDataAsset>> DataAssetOfAllTowers;
	
	/** Array containing all the towers name, making it easier to fetch data from table */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TArray<FName> TowerNames = { "CannonTower", "MachineGunTower", "ArcherTower", "CatapultTower" };

	/** Canon tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TSubclassOf<ASBaseTower> CanonTowerClass;

	/** Machine gun tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TSubclassOf<ASBaseTower> MachineGunTowerClass;

	/** Archer tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TSubclassOf<ASBaseTower> ArcherTowerClass;

	/** Catapult tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData)
	TSubclassOf<ASBaseTower> CatapultTowerClass;

#pragma endregion TowerData
	
	/** Ref to the tower site which owns this widget, for it to spawn the tower on that site */
	TWeakObjectPtr<ASTowerSite> OwningTowerSite;

	/**
	 * Called from the tower button callbacks
	 * Spawns the passed tower class
	 * Calls the player pawn class to deactivate the last active tower site
	 */
	void SpawnGivenTower(const TSubclassOf<ASBaseTower>& TowerToSpawn);

	/** 
	 * Ref to the player pawn
	 * It acts as the manager for the active tower site
	 * Through the player pawn we make the call to deactivate the current tower site
	 * As we have spawned the tower
	 */
	TWeakObjectPtr<ASTowerDefensePawn> PlayerPawn;

	/** Game mode ref to check if we have enough coins to spawn the given tower */
	TWeakObjectPtr<ASBaseGameMode> BaseGameMode;

	/** Returns true if player have enough coins to spawn the given tower and deducts them */
	bool CheckAndDeductIfEnoughCoins(const FName& InTowerName);

	/** Ref to the base HUD class to show error messages in viewport */
	TWeakObjectPtr<ASBaseHUD> BaseHUD;
	
	/** Map containing tower buying price against the tower name, set in constructor */
	TMap<FName, uint32> TowerPriceMap;

	/** Map containing tower icon against the tower name, set in constructor */
	TMap<FName, FSlateBrush> TowerIconMap;

	/** Callback when pop in anim finishes playing in reverse so that tower site can hide the widget component */
	UFUNCTION()
	void FinishedPlayingPopInAnim() const;

public:
	/** Called from owning ASTowerSite to play the pop in anim in forward or reverse */
	void PlayPopInAnimation(bool bIsReversed = false, const float& PlaybackSpeed = 1.f);

	/** Called from owning ASTowerSite to set a ref to itself so that we can get its location for tower to spawn */
	void SetOwningTowerSite(ASTowerSite* InOwningTowerSite);
	
};
