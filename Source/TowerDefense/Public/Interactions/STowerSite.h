
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STowerSite.generated.h"

class UWidgetComponent;
class USTowerSelectionMenu;
class ASTowerDefensePawn;

/**
 * Tower spawning site class
 * Places the chosen tower by the user upon itself
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Interactions)
class TOWERDEFENSE_API ASTowerSite : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor */
	ASTowerSite();

	/** Called after the components have been initialized, used to add bindings */
	virtual void PostInitializeComponents() override;

	/** 
	 * Called from ASTowerDefensePawn::SetCurrentlyActiveTowerSite
	 * If a tower site has widget active but user clicks on a different site or LMB/RMB is pressed anywhere on the map
	 * Indicating user no longer wants interact and so close the widget
	 */
	void SetTowerSiteToUnselected();

protected:
	/** 
	 * Begin play override
	 * Initializes the player controller
	 * Set the input mode to game
	 * Enables all the mouse bool like click and mouse over events
	 */
	virtual void BeginPlay() override;

private:

#pragma region ComponentsAndCallbacks
	
	/** Root component for this class */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Mesh for the tower site */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerSiteMesh;

	/** Widget component for the tower selection menu */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UWidgetComponent> TowerSelectionMenuWidgetComponent;

	/** Callback when this actor is clicked by the user */
	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

#pragma endregion ComponentsAndCallbacks
	
	/** Ref to the player pawn so that we call set the last selected tower site via interface */
	TWeakObjectPtr<ASTowerDefensePawn> PlayerPawn;
	
	/** Ref to the widget of the widget component for us to play the pop in animation */
	TWeakObjectPtr<USTowerSelectionMenu> TowerSelectionMenuWidget;

	/**
	 * True when the site is active
	 * Helps to avoid end cursor overlap when widget is activated
	 */
	bool bIsSiteActive = false;

	/**	True when site is disabled, when a tower is already placed on the site */
	bool bIsSiteDisabled = false;

	/**
	 * True when cursor is over this site
	 * Set from blueprints, when player clicks on the tower site when it is active this variable
	 * helps to check if player is still hovering, if true then it will not change the emissive value
	 * Used in ASTowerSite::SetTowerSiteToUnselected
	 */
	UPROPERTY(BlueprintReadWrite, Category = Defaults, meta = (AllowPrivateAccess = true))
	bool bIsCursorOver = false;
	
public:
	/** 
	 * Called from USTowerSelectionMenu::FinishedPlayingPopInAnim() to hide the widget component
	 * When the pop in animation of the tower selection menu finishes playing
	 */
	void HideTowerSelectionMenuWidgetComponent() const;
	
	/** Getter for TowerSiteMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerSiteMesh() const
	{ return TowerSiteMesh; }

	/** Getter for bIsSiteActive */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSiteActive() const
	{ return bIsSiteActive; }
	
	/** Getter for bIsSiteDisabled */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSiteDisabled() const
	{ return bIsSiteDisabled; }
	
	/** Setter for bIsSiteDisabled */
	FORCEINLINE void SetIsSiteDisabled(const bool Value)
	{ bIsSiteDisabled = Value; }
	
};
