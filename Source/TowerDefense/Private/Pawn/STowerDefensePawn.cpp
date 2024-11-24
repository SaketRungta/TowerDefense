
#include "Pawn/STowerDefensePawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "System/CommonTypes.h"
#include "Towers/SBaseTower.h"
#include "UI/SBaseHUD.h"

ASTowerDefensePawn::ASTowerDefensePawn()
{
 	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneRoot);
	CameraComponent->SetRelativeRotation(FRotator(-50.f, -90.f, 0.f));
}

void ASTowerDefensePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPanning) CameraPanImplementation();
}

void ASTowerDefensePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerController = PlayerController.IsValid() ? PlayerController : TObjectPtr<APlayerController>(Cast<APlayerController>(GetController()));
	if (PlayerController.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Started, this, &ThisClass::OnRightMouseButtonAction);
		EnhancedInputComponent->BindAction(RightMouseButtonAction, ETriggerEvent::Completed, this, &ThisClass::OnRightMouseButtonAction);

		EnhancedInputComponent->BindAction(LeftMouseButtonAction, ETriggerEvent::Started, this, &ThisClass::OnLeftMouseButtonAction);

		EnhancedInputComponent->BindAction(EscapeButtonAction, ETriggerEvent::Started, this, &ThisClass::OnEscapeButtonAction);

		EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Started, this, &ThisClass::OnMouseWheelAction);
	}
}

void ASTowerDefensePawn::SetCurrentlySelectedTowerSite(ASTowerSite* NewSelectedTowerSite)
{
	if (LastSelectedTowerSite.IsValid()) LastSelectedTowerSite->SetTowerSiteToUnselected();
	LastSelectedTowerSite = NewSelectedTowerSite;
}

void ASTowerDefensePawn::SetCurrentlySelectedTower(ASBaseTower* NewSelectedTower)
{
	if (LastSelectedTower.IsValid()) LastSelectedTower->SetTowerToUnselected();
	LastSelectedTower = NewSelectedTower;
}

void ASTowerDefensePawn::UnPauseTheGame()
{
	const FInputActionValue Value;
	OnEscapeButtonAction(Value);
}

void ASTowerDefensePawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
	PlayerController = PlayerController.IsValid() ? PlayerController : TObjectPtr<APlayerController>(Cast<APlayerController>(GetController()));
	if (PlayerController.IsValid())
	{		
		FInputModeGameAndUI InputModeData;
		InputModeData.SetHideCursorDuringCapture(false);
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PlayerController->SetInputMode(InputModeData);

		PlayerController->SetShowMouseCursor(true);
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

void ASTowerDefensePawn::OnRightMouseButtonAction(const FInputActionValue& Value)
{
	bIsPanning = Value.Get<bool>();
	if (bIsPanning)
	{
		SetCurrentlySelectedTowerSite(nullptr);
		SetCurrentlySelectedTower(nullptr);
		
		PlayerController = PlayerController.IsValid() ? PlayerController : TObjectPtr<APlayerController>(Cast<APlayerController>(GetController()));
		if (PlayerController.IsValid()) PlayerController->GetMousePosition(InitialMousePosition.X, InitialMousePosition.Y);
	}
	SetActorTickEnabled(Value.Get<bool>());
}

void ASTowerDefensePawn::OnLeftMouseButtonAction(const FInputActionValue& Value)
{
	PlayerController = PlayerController.IsValid() ? PlayerController : TObjectPtr<APlayerController>(Cast<APlayerController>(GetController()));
	if (PlayerController.IsValid())
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
		if(!HitResult.GetActor()->ActorHasTag(FName("TowerSite"))) SetCurrentlySelectedTowerSite(nullptr);
		if(!HitResult.GetActor()->ActorHasTag(FName("Tower"))) SetCurrentlySelectedTower(nullptr);
	}
}

void ASTowerDefensePawn::OnEscapeButtonAction(const FInputActionValue& Value)
{
	if (!bCanPauseTheGame) return;
	
	bIsGamePaused = !bIsGamePaused;
	UGameplayStatics::SetGamePaused(this, bIsGamePaused);

	EMenuToShow MenuToShow;
	
	if (bIsGamePaused) MenuToShow = EMenuToShow::Pause;
	else MenuToShow = EMenuToShow::HUD;
	
	BaseHUD = BaseHUD.IsValid() ? BaseHUD : TObjectPtr<ASBaseHUD>(Cast<ASBaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()));
	if(BaseHUD.IsValid()) BaseHUD->ShowTheGivenMenu(MenuToShow);
}

void ASTowerDefensePawn::OnMouseWheelAction(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	
	const FVector CurrentLocation = CameraComponent->GetComponentLocation();
	const FVector UpVector = CameraComponent->GetForwardVector();
	const FVector ZoomOffset = UpVector * AxisValue * CameraZoomingSpeed;
	const FVector NewLocation = CurrentLocation + ZoomOffset;
	if (NewLocation.Z >= MinHeight && NewLocation.Z <= MaxHeight)
		CameraComponent->SetWorldLocation(NewLocation);
}

void ASTowerDefensePawn::CameraPanImplementation()
{
	FVector2D CurrentMousePosition;

	PlayerController = PlayerController.IsValid() ? PlayerController : TObjectPtr<APlayerController>(Cast<APlayerController>(GetController()));
	if (PlayerController.IsValid()) PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	
	FVector DeltaLocation;
	DeltaLocation.X = (InitialMousePosition.X - CurrentMousePosition.X) * CameraPanSpeed;
	DeltaLocation.Y = (InitialMousePosition.Y - CurrentMousePosition.Y) * CameraPanSpeed;

	AddActorWorldOffset(DeltaLocation, true);
}
