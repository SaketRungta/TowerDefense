
#include "UI/STowerSelectionMenu.h"
#include "UI/STowerSelectionMenuButton.h"
#include "Components/Button.h"
#include "Towers/SBaseTower.h"
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/SGameInteractionInterface.h"
#include "GameFramework/Pawn.h"
#include "GameMode/SBaseGameMode.h"
#include "GameFramework/HUD.h"

bool USTowerSelectionMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (CannonButton) CannonButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCannonButtonClicked);
	if (MachineGunButton) MachineGunButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnMachineGunButtonClicked);
	if (ArcherTowerButton) ArcherTowerButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnArcherTowerButtonClicked);
	if (CatapultButton) CatapultButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCatapultButtonClicked);

	return true;
}

void USTowerSelectionMenu::OnCannonButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(CannonButton->GetTowerPrice())) return;
	SpawnGivenTower(CanonTowerClass);
}

void USTowerSelectionMenu::OnMachineGunButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(MachineGunButton->GetTowerPrice())) return;
	SpawnGivenTower(MachineGunTowerClass);
}

void USTowerSelectionMenu::OnArcherTowerButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(ArcherTowerButton->GetTowerPrice())) return;
	SpawnGivenTower(ArcherTowerClass);
}

void USTowerSelectionMenu::OnCatapultButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(CatapultButton->GetTowerPrice())) return;
	SpawnGivenTower(CatapultTowerClass);
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

	/** A tower has been spawned on the site so we need to disable it otherwise it will respond to player clicks and hovers */
	OwningTowerSite->SetIsSiteDisabled(true);
	
	PlayerPawn = PlayerPawn.IsValid() == true ? PlayerPawn : (UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn.IsValid())
	{
		if (PlayerPawn->GetClass()->ImplementsInterface(USGameInteractionInterface::StaticClass()))
		{
			GameInteractionInterfaceToPlayerPawn = GameInteractionInterfaceToPlayerPawn != nullptr? GameInteractionInterfaceToPlayerPawn : Cast<ISGameInteractionInterface>(PlayerPawn);
			if (GameInteractionInterfaceToPlayerPawn != nullptr) GameInteractionInterfaceToPlayerPawn->SetCurrentlySelectedTowerSite(nullptr);
		}
	}
}

bool USTowerSelectionMenu::CheckAndDeductIfEnoughCoins(const uint32& InTowerPrice)
{
	BaseGameMode = BaseGameMode.IsValid() ? BaseGameMode : Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!BaseGameMode.IsValid()) return false;
	if(BaseGameMode.IsValid() && BaseGameMode->DeductCoins(InTowerPrice)) return true;

	BaseHUD = BaseHUD.IsValid() ? BaseHUD : GetWorld()->GetFirstPlayerController()->GetHUD();
	if(BaseHUD.IsValid())
	{
		if (BaseHUD->GetClass()->ImplementsInterface(USGameInteractionInterface::StaticClass()))
		{
			GameInteractionInterfaceToBaseHUD = GameInteractionInterfaceToBaseHUD != nullptr ? GameInteractionInterfaceToBaseHUD : Cast<ISGameInteractionInterface>(BaseHUD);
			if (GameInteractionInterfaceToBaseHUD != nullptr) GameInteractionInterfaceToBaseHUD->ShowErrorMessage(FString("Not Enough Coins"));
		}
	}
	
	return false;
}

void USTowerSelectionMenu::SetOwningTowerSite(ASTowerSite* InOwningTowerSite)
{
	OwningTowerSite = InOwningTowerSite;
}
