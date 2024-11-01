
#include "Interactions/STowerSite.h"
#include "Interface/SPlayerPawnInterface.h"
#include "Components/WidgetComponent.h"
#include "UI/STowerSelectionMenu.h"

ASTowerSite::ASTowerSite()
{
 	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TowerSiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerSiteMesh"));
	TowerSiteMesh->SetupAttachment(SceneRoot);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(SceneRoot);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(375.f, 375.f));
	WidgetComponent->SetHiddenInGame(true);

	Tags.Add(FName(TEXT("TowerSite")));
}

void ASTowerSite::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->OnClicked.AddDynamic(this, &ThisClass::OnActorClicked);
}

void ASTowerSite::SetTowerSiteToUnselected()
{
	TowerSelectionMenu = TowerSelectionMenu.IsValid() == true ? TowerSelectionMenu : Cast<USTowerSelectionMenu>(WidgetComponent->GetWidget());
	if (!TowerSelectionMenu.IsValid()) return;

	TowerSelectionMenu->PlayPopInAnimation(true);
	bIsSiteActive = false;
	GetTowerSiteMesh()->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), 0.f);

	FTimerHandle PopInAnimFinishTimer;
	GetWorld()->GetTimerManager().SetTimer(
		PopInAnimFinishTimer,
		[this]() {
			WidgetComponent->SetHiddenInGame(true);
		},
		WidgetPopInAnimPlayDuration,
		false
	);
}

void ASTowerSite::BeginPlay()
{
	Super::BeginPlay();
	
	TowerSelectionMenu = Cast<USTowerSelectionMenu>(WidgetComponent->GetWidget());
	if (TowerSelectionMenu.IsValid()) TowerSelectionMenu->SetOwningTowerSite(this);
}

void ASTowerSite::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if(bIsSiteDisabled) return;

#pragma region InterfaceCall
	
	static APawn* PlayerPawn = nullptr;
	static ISPlayerPawnInterface* PlayerPawnInterface = nullptr;

	// Check if PlayerPawn is valid, otherwise try to get it
	if (!PlayerPawn || !PlayerPawn->IsValidLowLevel())
	{
		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	if (PlayerPawn && PlayerPawn->IsValidLowLevel())
	{
		// Check if PlayerPawn implements the interface and set PlayerPawnInterface if it hasn't been set
		if (PlayerPawn->GetClass()->ImplementsInterface(USPlayerPawnInterface::StaticClass()))
		{
			if (!PlayerPawnInterface)
			{
				PlayerPawnInterface = Cast<ISPlayerPawnInterface>(PlayerPawn);
			}

			// Ensure PlayerPawnInterface is valid before calling the function
			if (PlayerPawnInterface)
			{
				PlayerPawnInterface->SetCurrentlySelectedTowerSite(this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerPawnInterface is nullptr, cannot call SetCurrentlySelectedTowerSite."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is nullptr or invalid."));
	}

#pragma endregion InterfaceCall
	
	TowerSelectionMenu = TowerSelectionMenu.IsValid() == true ? TowerSelectionMenu : Cast<USTowerSelectionMenu>(WidgetComponent->GetWidget());
	if (!TowerSelectionMenu.IsValid()) return;

	WidgetComponent->SetHiddenInGame(false);

	TowerSelectionMenu->PlayPopInAnimation();
	bIsSiteActive = true;
}
