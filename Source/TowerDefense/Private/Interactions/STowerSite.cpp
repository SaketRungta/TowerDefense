
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
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

void ASTowerSite::DeactivateTowerSite()
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
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TowerSelectionMenu = Cast<USTowerSelectionMenu>(WidgetComponent->GetWidget());
	if (TowerSelectionMenu.IsValid()) TowerSelectionMenu->SetOwningTowerSite(this);
}

void ASTowerSite::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{	
	PlayerPawn = PlayerPawn.IsValid() == true ? PlayerPawn : UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn.IsValid())
	{
		if (PlayerPawn->GetClass()->ImplementsInterface(USPlayerPawnInterface::StaticClass()))
		{
			PlayerPawnInterface = PlayerPawnInterface != nullptr? PlayerPawnInterface : Cast<ISPlayerPawnInterface>(PlayerPawn);
			if (PlayerPawnInterface != nullptr) PlayerPawnInterface->SetCurrentlyActiveTowerSite(this);
		}
	}

	TowerSelectionMenu = TowerSelectionMenu.IsValid() == true ? TowerSelectionMenu : Cast<USTowerSelectionMenu>(WidgetComponent->GetWidget());
	if (!TowerSelectionMenu.IsValid()) return;

	WidgetComponent->SetHiddenInGame(false);

	TowerSelectionMenu->PlayPopInAnimation();
	bIsSiteActive = true;
}
