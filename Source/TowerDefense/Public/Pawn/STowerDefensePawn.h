
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/SPlayerPawnInterface.h"
#include "STowerDefensePawn.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASBaseTower;

/**
 * Main pawn class
 * It contains the camera and handles the input 
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASTowerDefensePawn : public APawn, public ISPlayerPawnInterface
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ASTowerDefensePawn();

	/** Tick override */
	virtual void Tick(float DeltaTime) override;

	/** Setups the input and related callbacks for each input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called from ASTowerSite::OnActorClicked to set the currently selected tower site */
	virtual void SetCurrentlySelectedTowerSite(ASTowerSite* NewSelectedTowerSite) override;

	/** Called from ASBaseTower::OnActorClicked to set the currently selected tower */
	virtual void SetCurrentlySelectedTower(ASBaseTower* NewSelectedTower) override;
	
protected:
	/**
	 * Begin play override
	 * Initializes the player controller
	 * Set the input mode to game
	 * Enables all the mouse bool like click and mouse over events
	 */
	virtual void BeginPlay() override;

private:
	/** Scene root component for this object */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	/** Main camera for the pawn to render to viewport */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

#pragma region Input

	/** Default input mapping context for this pawn */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Input action for the right mouse button press and release */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RightMouseButtonAction;

	/** Input action for the left mouse button press and release */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LeftMouseButtonAction;

	/** Called when right mouse button is pressed or released */
	void OnRightMouseButtonAction(const FInputActionValue& Value);

	/** Called when left mouse button is pressed, checks if the left mouse button is not pressed on tower site if true then deactivates any active tower site */
	void OnLeftMouseButtonAction(const FInputActionValue& Value);

#pragma endregion Input

	/** Default player controller ref, used to set up the input mapping context and to get the mouse position when required */
	TWeakObjectPtr<APlayerController> PlayerController;

#pragma region CameraPanning

	/** True when right mouse button is pressed for camera panning action */
	bool bIsPanning = false;

	/** Moves the camera as per the change in mouse potion to give the camera panning effect, called by event tick */
	void CameraPanImplementation();

	/** Stores the mouse position when player presses the right mouse button to start camera panning */
	FVector2d InitialMousePosition{ 0.f, 0.f };

	/** Speed at which camera should pan */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (ClampMin = "0.001", ClampMax = "1"))
	float CameraPanSpeed = 0.01f;

#pragma endregion CameraPanning

	/** 
	 * Stores the tower site which was selected by the user
	 * When user selects a new tower site "SetCurrentlySelectedTowerSite" function sets the variable with newly selected tower site
	 * If this tower site is unselected then it is set back to nullptr
	 */
	TWeakObjectPtr<ASTowerSite> LastSelectedTowerSite;
	
	/** 
	 * Stores the tower which was last selected by the user
	 * When user selects a new tower "SetCurrentlySelectedTower" function sets the variable with newly selected tower
	 * If this tower is unselected then it is set to nullptr
	 */
	TWeakObjectPtr<ASBaseTower> LastSelectedTower;
};
