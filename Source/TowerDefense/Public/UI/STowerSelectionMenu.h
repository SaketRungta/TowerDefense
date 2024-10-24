
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STowerSelectionMenu.generated.h"

class USTowerSelectionMenuButton;
class ASTowerSite;
class ASBaseTower;
class ISPlayerPawnInterface;
class APawn;

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

private:
	/** Canon tower selection button */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USTowerSelectionMenuButton> CannonButton;

	/** Machine gun tower selection button */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USTowerSelectionMenuButton> MachineGunButton;

	/** Archer tower selection button */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USTowerSelectionMenuButton> ArcherTowerButton;

	/** Catapult tower selection button */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USTowerSelectionMenuButton> CatapultButton;

	/** Ref to the tower site which owns this widget, for it to spawn the tower on that site */
	TWeakObjectPtr<ASTowerSite> OwningTowerSite;

	/** Canon tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASBaseTower> CanonTowerClass;

	/** Machine gun tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASBaseTower> MachineGunTowerClass;

	/** Archer tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASBaseTower> ArcherTowerClass;

	/** Catapult tower to spawn, to be initialized from the blueprint */
	UPROPERTY(EditDefaultsOnly, Category = TowerData, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASBaseTower> CatapultTowerClass;

	/** Callback when user clicks on the canon tower button */
	UFUNCTION()
	void OnCannonButtonClicked()
	{ SpawnGivenTower(CanonTowerClass); }

	/** Callback when user clicks on the machine gun tower button */
	UFUNCTION()
	void OnMachineGunButtonClicked()
	{ SpawnGivenTower(MachineGunTowerClass); }

	/** Callback when user clicks on the archer tower button */
	UFUNCTION()
	void OnArcherTowerButtonClicked()
	{ SpawnGivenTower(ArcherTowerClass); }

	/** Callback when user clicks on the catapult tower button */
	UFUNCTION()
	void OnCatapultButtonClicked()
	{ SpawnGivenTower(CatapultTowerClass); }

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
	 */
	ISPlayerPawnInterface* PlayerPawnInterface;

public:
	/** Called from owning ASTowerSite to play the pop in anim in forward or reverse */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPopInAnimation(bool bIsReversed = false);

	/** Called from owning ASTowerSite to set a ref to itself so that we can get its location for tower to spawn */
	void SetOwningTowerSite(ASTowerSite* InOwningTowerSite);
};
