
#include "Interactions/STowerSite.h"

#include "Components/WidgetComponent.h"
#include "Pawn/STowerDefensePawn.h"
#include "UI/STowerSelectionMenu.h"

ASTowerSite::ASTowerSite()
{
 	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TowerSiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerSiteMesh"));
	TowerSiteMesh->SetupAttachment(SceneRoot);

	TowerSelectionMenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerSelectionMenuWidgetComponent"));
	TowerSelectionMenuWidgetComponent->SetupAttachment(SceneRoot);
	TowerSelectionMenuWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TowerSelectionMenuWidgetComponent->SetDrawSize(FVector2D(375.f, 375.f));
	TowerSelectionMenuWidgetComponent->SetHiddenInGame(true);

	Tags.Add(FName(TEXT("TowerSite")));
}

void ASTowerSite::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->OnClicked.AddDynamic(this, &ThisClass::OnActorClicked);
}

void ASTowerSite::SetTowerSiteToUnselected()
{
	TowerSelectionMenuWidget = TowerSelectionMenuWidget.IsValid() ? TowerSelectionMenuWidget : TObjectPtr<USTowerSelectionMenu>(Cast<USTowerSelectionMenu>(TowerSelectionMenuWidgetComponent->GetWidget()));
	if (!TowerSelectionMenuWidget.IsValid()) return;

	TowerSelectionMenuWidget->PlayPopInAnimation(true, 2.f);
	bIsSiteActive = false;
	if (!bIsCursorOver)
		GetTowerSiteMesh()->SetScalarParameterValueOnMaterials(FName(TEXT("Emissive")), 0.f);
}

void ASTowerSite::BeginPlay()
{
	Super::BeginPlay();
	
	TowerSelectionMenuWidget = TObjectPtr<USTowerSelectionMenu>(Cast<USTowerSelectionMenu>(TowerSelectionMenuWidgetComponent->GetWidget()));
	if (TowerSelectionMenuWidget.IsValid()) TowerSelectionMenuWidget->SetOwningTowerSite(this);
}

void ASTowerSite::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (bIsSiteDisabled) return;

	if (!bIsSiteActive)
	{
		PlayerPawn = PlayerPawn.IsValid() ? PlayerPawn : TObjectPtr<ASTowerDefensePawn>(Cast<ASTowerDefensePawn>(GetWorld()->GetFirstPlayerController()->GetPawn()));
		if (PlayerPawn.IsValid()) PlayerPawn->SetCurrentlySelectedTowerSite(this);
	
		TowerSelectionMenuWidget = TowerSelectionMenuWidget.IsValid() ? TowerSelectionMenuWidget : TObjectPtr<USTowerSelectionMenu>(Cast<USTowerSelectionMenu>(TowerSelectionMenuWidgetComponent->GetWidget()));
		if (TowerSelectionMenuWidget.IsValid()) TowerSelectionMenuWidget->PlayPopInAnimation(false, 2.f);

		TowerSelectionMenuWidgetComponent->SetHiddenInGame(false);

		bIsSiteActive = true;
	}
	else
	{
		PlayerPawn = PlayerPawn.IsValid() ? PlayerPawn : TObjectPtr<ASTowerDefensePawn>(Cast<ASTowerDefensePawn>(GetWorld()->GetFirstPlayerController()->GetPawn()));
		if (PlayerPawn.IsValid()) PlayerPawn->SetCurrentlySelectedTowerSite(nullptr);
	
		TowerSelectionMenuWidget = TowerSelectionMenuWidget.IsValid() ? TowerSelectionMenuWidget : TObjectPtr<USTowerSelectionMenu>(Cast<USTowerSelectionMenu>(TowerSelectionMenuWidgetComponent->GetWidget()));
		if (TowerSelectionMenuWidget.IsValid()) TowerSelectionMenuWidget->PlayPopInAnimation(true, 2.f);

		bIsSiteActive = false;
	}
}

void ASTowerSite::HideTowerSelectionMenuWidgetComponent() const
{
	TowerSelectionMenuWidgetComponent->SetHiddenInGame(true);
}
