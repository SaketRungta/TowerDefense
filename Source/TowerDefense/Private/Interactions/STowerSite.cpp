
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPlayerPawnInterface.h"
#include "Components/WidgetComponent.h"

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
}

void ASTowerSite::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->OnClicked.AddDynamic(this, &ThisClass::OnActorClicked);
}

void ASTowerSite::DeactivateTowerSite()
{
}

void ASTowerSite::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	WidgetComponent->GetWidget();
}

void ASTowerSite::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{	
	UE_LOG(LogTemp, Warning, TEXT("ASTowerSite::OnTowerSiteMeshClicked"));
	PlayerPawn = PlayerPawn.IsValid() == true ? PlayerPawn : UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn.IsValid())
	{
		if (PlayerPawn->GetClass()->ImplementsInterface(USPlayerPawnInterface::StaticClass()))
		{
			ISPlayerPawnInterface* PlayerPawnInterface = Cast<ISPlayerPawnInterface>(PlayerPawn);
			if (PlayerPawnInterface)
			{
				PlayerPawnInterface->SetCurrentlyActiveTowerSite(this);
			}
		}
	}
}
