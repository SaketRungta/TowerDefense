
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STowerSite.generated.h"

class UWidgetComponent;
class USTurretSelectionMenu;
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
	void DeactivateTowerSite();

protected:
	/** 
	 * Begin play override
	 * Intializes the player controller
	 * Set the input mode to game
	 * Enables all the mouse bools like click and mouse over events
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

	/** 
	 * Ref to the player pawn
	 * Player pawn acts as the manager for the active tower site
	 * If a tower site is active and user clicks on another tower then we need to deactive the last active tower
	 * We also need to tell the pawn that we are now the active tower
	 * We do it all using an interface to reduce calls so using APawn not ASTowerDefensePawn
	 */
	TWeakObjectPtr<APawn> PlayerPawn;

	/** Ref to the widget of the widget component for us to play the the pop in animation */
	TWeakObjectPtr<USTurretSelectionMenu> TurretSelectionMenu;

	/** 
	 * Interface ref of the player pawn class ASTowerDefensePawn
	 * We will clicking multiple times on the same tower site
	 * To reduce casting everytime we click on the site we store it as a ref here
	 */
	ISPlayerPawnInterface* PlayerPawnInterface;

	/** 
	 * Duration it takes for the widget to play the pop in animation 
	 * A timer fires once widget pop in anim is played in revered so that widget component can be set to hidden in game
	 */
	UPROPERTY(EditAnywhere, Category = Widget, meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "1"))
	float WidgetPopInAnimPlayDuration = 0.25f;

public:
	/** Getter for TowerSiteMesh */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetTowerSiteMesh() const { return TowerSiteMesh; }
};
