
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STowerSite.generated.h"

class UWidgetComponent;
class USTowerSelectionMenu;
class ISPlayerPawnInterface;

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
	/** Root component for this class */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Mesh for the tower site */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> TowerSiteMesh;

	/** Widget component for the tower selection menu */
	UPROPERTY(EditDefaultsOnly, Category = Components)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/** Callback when this actor is clicked by the user */
	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	/** Ref to the widget of the widget component for us to play the pop in animation */
	TWeakObjectPtr<USTowerSelectionMenu> TowerSelectionMenu;

	/** 
	 * Duration it takes for the widget to play the pop in animation 
	 * A timer fires once widget pop in anim is played in revered so that widget component can be set to hidden in game
	 */
	UPROPERTY(EditAnywhere, Category = Widget, meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "1"))
	float WidgetPopInAnimPlayDuration = 0.25f;

	/**
	 * True when the site is active
	 * Helps to avoid end cursor overlap when widget is activated
	 */
	bool bIsSiteActive = false;

	/**	True when site is disabled, when a tower is already placed on the site */
	bool bIsSiteDisabled = false;
	
public:
	/** Getter for TowerSiteMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerSiteMesh() const { return TowerSiteMesh; }

	/** Getter for bIsSiteActive */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSiteActive() const { return bIsSiteActive; }
	
	/** Getter for bIsSiteDisabled */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSiteDisabled() const { return bIsSiteDisabled; }
	
	/** Setter for bIsSiteDisabled */
	FORCEINLINE void SetIsSiteDisabled(const bool Value) { bIsSiteDisabled = Value; }
	
};
