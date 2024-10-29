
#include "Pawn/STowerDefensePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interactions/STowerSite.h"
#include "Towers/SBaseTower.h"

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

	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
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

void ASTowerDefensePawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());

	if (PlayerController.IsValid())
	{
		FInputModeGameOnly InputModeData;
		InputModeData.SetConsumeCaptureMouseDown(false);
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
		PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
		if(PlayerController.IsValid()) PlayerController->GetMousePosition(InitialMousePosition.X, InitialMousePosition.Y);
	}
	SetActorTickEnabled(Value.Get<bool>());
}

void ASTowerDefensePawn::OnLeftMouseButtonAction(const FInputActionValue& Value)
{
	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
	if (PlayerController.IsValid())
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
		if(!HitResult.GetActor()->ActorHasTag(FName("TowerSite"))) SetCurrentlySelectedTowerSite(nullptr);
		if(!HitResult.GetActor()->ActorHasTag(FName("Tower"))) SetCurrentlySelectedTower(nullptr);
	}
}

void ASTowerDefensePawn::CameraPanImplementation()
{
	FVector2D CurrentMousePosition;

	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
	if (PlayerController.IsValid()) PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	
	FVector DeltaLocation;
	DeltaLocation.X = (InitialMousePosition.X - CurrentMousePosition.X) * CameraPanSpeed;
	DeltaLocation.Y = (InitialMousePosition.Y - CurrentMousePosition.Y) * CameraPanSpeed;

	AddActorWorldOffset(DeltaLocation, true);
}
