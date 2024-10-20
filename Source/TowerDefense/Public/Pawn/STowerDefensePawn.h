
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STowerDefensePawn.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * Main pawn class
 * Pawn class containing the camera and handles the input 
 ********************************************************************************************/
UCLASS(BlueprintAble, BlueprintType, ClassGroup = Projectile)
class TOWERDEFENSE_API ASTowerDefensePawn : public APawn
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ASTowerDefensePawn();

	/** Tick override */
	virtual void Tick(float DeltaTime) override;

	/** Setups the input and related callbacks for each input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Begin play override */
	virtual void BeginPlay() override;

	/** Scene root component for this object */
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	/** Main camera for the pawn to render to viewport */
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

private:

#pragma region Input

	/** Default input mapping context for this pawn */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Input action for the any button press and release */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RightMouseButtonAction;

	/** Called when right mouse button is pressed or released */
	void OnRightMouseButtonAction(const FInputActionValue& Value);

#pragma endregion Input

	/** Default player controller ref, used to setup the input mapping context and to get the mouse position when required */
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
};
