
#include "UI/STowerSelectionMenu.h"
#include "UI/STowerSelectionMenuButton.h"
#include "Components/Button.h"
#include "Towers/SBaseTower.h"
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SPlayerPawnInterface.h"
#include "GameFramework/Pawn.h"

bool USTowerSelectionMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (CannonButton) CannonButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCannonButtonClicked);
	if (MachineGunButton) MachineGunButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnMachineGunButtonClicked);
	if (ArcherTowerButton) ArcherTowerButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnArcherTowerButtonClicked);
	if (CatapultButton) CatapultButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCatapultButtonClicked);

	return true;
}

void USTowerSelectionMenu::SpawnGivenTower(const TSubclassOf<ASBaseTower>& TowerToSpawn)
{
	if (!OwningTowerSite.IsValid() || !TowerToSpawn->IsValidLowLevel()) return;

	FTransform SpawnTransform = OwningTowerSite->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation();
	SpawnLocation.Z += 60.f;
	SpawnTransform.SetLocation(SpawnLocation);

	GetWorld()->SpawnActor<ASBaseTower>(
		TowerToSpawn,
		SpawnTransform
	);

	OwningTowerSite->SetIsSiteDisabled(true);
	
	PlayerPawn = PlayerPawn.IsValid() == true ? PlayerPawn : (UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn.IsValid())
	{
		if (PlayerPawn->GetClass()->ImplementsInterface(USPlayerPawnInterface::StaticClass()))
		{
			PlayerPawnInterface = PlayerPawnInterface != nullptr? PlayerPawnInterface : Cast<ISPlayerPawnInterface>(PlayerPawn);
			if (PlayerPawnInterface != nullptr) PlayerPawnInterface->SetCurrentlyActiveTowerSite(nullptr);
		}
	}
}

void USTowerSelectionMenu::SetOwningTowerSite(ASTowerSite* InOwningTowerSite)
{
	OwningTowerSite = InOwningTowerSite;
}
