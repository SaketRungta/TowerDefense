
#include "Pawn/STowerDefensePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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
	}
}

void ASTowerDefensePawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
}

void ASTowerDefensePawn::OnRightMouseButtonAction(const FInputActionValue& Value)
{
	bIsPanning = Value.Get<bool>();
	if (bIsPanning)
	{
		PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
		PlayerController->GetMousePosition(InitialMousePosition.X, InitialMousePosition.Y);
	}
	SetActorTickEnabled(Value.Get<bool>());
}

void ASTowerDefensePawn::CameraPanImplementation()
{
	FVector2D CurrentMousePostion;

	PlayerController = PlayerController.IsValid() == true ? PlayerController : Cast<APlayerController>(GetController());
	if (PlayerController.IsValid()) PlayerController->GetMousePosition(CurrentMousePostion.X, CurrentMousePostion.Y);
	
	FVector DeltaLocation;
	DeltaLocation.X = (InitialMousePosition.X - CurrentMousePostion.X) * CameraPanSpeed;
	DeltaLocation.Y = (InitialMousePosition.Y - CurrentMousePostion.Y) * CameraPanSpeed;

	AddActorWorldOffset(DeltaLocation, true);
}
