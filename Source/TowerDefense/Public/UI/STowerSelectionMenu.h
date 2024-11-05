
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenu.generated.h"

class USTowerSelectionMenuButton;
class ASTowerSite;
class ASBaseTower;
class ISGameInteractionInterface;
class APawn;
class ASBaseGameMode;
class USErrorPopUpWidget;
class AHUD;

/**
 * Menu for the user to select tower from
 * This menu pops up when the user clicks on a tower site to select the tower he wishes to spawn
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USTowerSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Default Constructor */
	USTowerSelectionMenu(const FObjectInitializer& ObjectInitializer);
	
	/** Used to initialize widget components with bindings */
	virtual bool Initialize() override;

private:
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

	/** Ref to the tower site which owns this widget, for it to spawn the tower on that site */
	TObjectPtr<ASTowerSite> OwningTowerSite;

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

	/**
	 * Called from the tower button callbacks
	 * Spawns the passed tower class
	 * Calls the player pawn class to deactivate the last active tower site
	 */
	void SpawnGivenTower(const TSubclassOf<ASBaseTower>& TowerToSpawn);

	/** 
	 * Ref to the player pawn
	 * It acts as the manager for the active tower site
	 * Through the player pawn we make the interface call to deactivate the current tower site
	 * As we have spawned the tower
	 */
	TWeakObjectPtr<APawn> PlayerPawn;

	/** 
	 * Interface ref of the player pawn class ASTowerDefensePawn
	 * We will be clicking multiple times on the same tower site
	 * To reduce casting everytime we click on the site we store it as a ref here
	 * Separate as base hud does not implement SetCurrentlySelectedTowerSite
	 */
	ISGameInteractionInterface* GameInteractionInterfaceToPlayerPawn = nullptr;

	/** Game mode ref to check if we have enough coins to spawn the given tower */
	TWeakObjectPtr<ASBaseGameMode> BaseGameMode;

	/** Returns true if player have enough coins to spawn the given tower and deducts them */
	bool CheckAndDeductIfEnoughCoins(const FName& InTowerName);

	/** Ref to the base HUD class to show error messages in viewport */
	TWeakObjectPtr<AHUD> BaseHUD;
	
	/** 
	 * Interface ref of the base HUD
	 * We will be clicking multiple times on the same button
	 * To reduce casting everytime we click on the button we store it as a ref here
	 * Separate as player pawn does not implement ShowErrorMessage
	 */
	ISGameInteractionInterface* GameInteractionInterfaceToBaseHUD = nullptr;

	/** Array containing all the towers name, making it easier to fetch data from table */
	TArray<FName> TowerNames = { "CannonTower", "MachineGunTower", "ArcherTower", "CatapultTower" };

	/** Map containing tower buying price against the tower name, set in constructor */
	TMap<FName, uint32> TowerPriceMap;
	
public:
	/** Called from owning ASTowerSite to play the pop in anim in forward or reverse */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPopInAnimation(bool bIsReversed = false);

	/** Called from owning ASTowerSite to set a ref to itself so that we can get its location for tower to spawn */
	void SetOwningTowerSite(ASTowerSite* InOwningTowerSite);
	
};
