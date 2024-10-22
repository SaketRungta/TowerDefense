
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPlayerPawnInterface.h"
#include "Components/WidgetComponent.h"
#include "UI/STurretSelectionMenu.h"

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
	TurretSelectionMenu = TurretSelectionMenu.IsValid() == true ? TurretSelectionMenu : Cast<USTurretSelectionMenu>(WidgetComponent->GetWidget());
	if (TurretSelectionMenu.IsValid())
	{
		TurretSelectionMenu->PlayPopInAnimation(true);
	}
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
	TurretSelectionMenu = Cast<USTurretSelectionMenu>(WidgetComponent->GetWidget());
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
	WidgetComponent->SetHiddenInGame(false);
	TurretSelectionMenu = TurretSelectionMenu.IsValid() == true ? TurretSelectionMenu : Cast<USTurretSelectionMenu>(WidgetComponent->GetWidget());
	if (TurretSelectionMenu.IsValid())
	{
		TurretSelectionMenu->PlayPopInAnimation();
	}
}
