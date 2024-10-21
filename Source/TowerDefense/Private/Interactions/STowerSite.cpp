
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPlayerPawnInterface.h"

ASTowerSite::ASTowerSite()
{
 	PrimaryActorTick.bCanEverTick = false;

	TowerSiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerSiteMesh"));
	SetRootComponent(TowerSiteMesh);
}

void ASTowerSite::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->OnClicked.AddDynamic(this, &ThisClass::OnActorClicked);
}

void ASTowerSite::DeactivateTowerSite()
{
	SetMaterial(1);
}

void ASTowerSite::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("<===ERROR===> ASTowerSite::OnTowerSiteMeshClicked PlayerPawn.IsValid"));
	}
	SetMaterial(0);
}
