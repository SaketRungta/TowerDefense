#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STowerDefensePawn.generated.h"

class UInputComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASBaseTower;
class ASBaseHUD;
class ASTowerSite;

/**
 * Main pawn class
 * It contains the camera and handles the input 
 ********************************************************************************************/
UCLASS(Blueprintable, BlueprintType, ClassGroup = Pawn)
class TOWERDEFENSE_API ASTowerDefensePawn : public APawn
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ASTowerDefensePawn();

	/** Tick override */
	virtual void Tick(float DeltaTime) override;

	/** Setups the input and related callbacks for each input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Called from ASTowerSite::OnActorClicked to set the currently selected tower site */
	void SetCurrentlySelectedTowerSite(ASTowerSite* NewSelectedTowerSite);

	/** Called from ASBaseTower::OnActorClicked to set the currently selected tower */
	void SetCurrentlySelectedTower(ASBaseTower* NewSelectedTower);

	/** Called from ASBaseGameMode::OnUFOReachedBaseCallback, when all lives are over and player can no longer pause the game */
	void SetCanPauseTheGame(const bool InCanPauseGame)
	{ bCanPauseTheGame = InCanPauseGame; }

	/** Called from the main HUD when paused to unpause the game */
	UFUNCTION(BlueprintCallable)
	void UnPauseTheGame();
	
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

	/** Input action for the left mouse button press */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LeftMouseButtonAction;

	/** Input action for the escape button press */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> EscapeButtonAction;

	/** Input action for the mouse wheel scrolling action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseWheelAction;

	/** Called when right mouse button is pressed or released */
	void OnRightMouseButtonAction(const FInputActionValue& Value);

	/** Called when left mouse button is pressed, checks if the left mouse button is not pressed on tower site if true then deactivates any active tower site */
	void OnLeftMouseButtonAction(const FInputActionValue& Value);

	/** Called when escape button is pressed, pauses the game */
	void OnEscapeButtonAction(const FInputActionValue& Value);

	/** Called when mouse wheel is scrolled, zooms in and out */
	void OnMouseWheelAction(const FInputActionValue& Value);

#pragma endregion Input

#pragma region CameraPanning

	/** True when right mouse button is pressed for camera panning action */
	bool bIsPanning = false;

	/** Moves the camera as per the change in mouse potion to give the camera panning effect, called by event tick */
	void CameraPanImplementation();

	/** Stores the mouse position when player presses the right mouse button to start camera panning */
	FVector2d InitialMousePosition{0.f, 0.f};

	/** Speed at which camera should pan */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (ClampMin = "0.001", ClampMax = "1"))
	float CameraPanSpeed = 0.01f;

	/** Minimum bounds of the camera */
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector CameraMinBounds = FVector(-600.f, -1500.f, 0.f);
	
	/** Minimum bounds of the camera */
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector CameraMaxBounds = FVector(600.f, 1500.f, 5000.f);

#pragma endregion CameraPanning

	/** Speed at which the camera zooms in out */
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (ClampMin = "1", ClampMax = "100"))
	float CameraZoomingSpeed = 15.f;
	
	/** Speed at which the camera zooms in out */
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (ClampMin = "1401", ClampMax = "6400"))
	float MaxHeight = 2000.f;
	
	/** Speed at which the camera zooms in out */
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (ClampMin = "400", ClampMax = "1399"))
	float MinHeight = 800.f;
	
	/** Default player controller ref, used to set up the input mapping context and to get the mouse position when required */
	TWeakObjectPtr<APlayerController> PlayerController;

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

	/** True when game is paused, helps to pause/unpause the game */
	bool bIsGamePaused = false;

	/** Ref to the base HUD class to show the pause menu */
	TWeakObjectPtr<ASBaseHUD> BaseHUD;

	/** False when either the level is completed or failed, so that user cannot pause once game is over */
	bool bCanPauseTheGame = true;
	
};
