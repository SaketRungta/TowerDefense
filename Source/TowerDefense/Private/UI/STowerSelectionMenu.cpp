
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

USTowerSelectionMenu::USTowerSelectionMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("DataTable'/Game/TowerDefense/Blueprints/DataTable/DT_TowerData.DT_TowerData'"));
	if (Asset.Succeeded())
		if (const UDataTable* TowerDataTable = Asset.Object)
			for (const FName& RowName : TowerDataTable->GetRowNames())
				if (const FTowerDataTableRow* RowData = TowerDataTable->FindRow<FTowerDataTableRow>(RowName, TEXT("")))
					TowerPriceMap.Add(RowName, RowData->TowerBuyingPrice);
}

bool USTowerSelectionMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (CannonButton)
	{
		CannonButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCannonButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[0]))
			CannonButton->SetTowerPrice(TowerPriceMap[TowerNames[0]]);
	}
	if (MachineGunButton)
	{
		MachineGunButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnMachineGunButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[1]))
			MachineGunButton->SetTowerPrice(TowerPriceMap[TowerNames[1]]);
	}
	if (ArcherTowerButton)
	{
		ArcherTowerButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnArcherTowerButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[2]))
			ArcherTowerButton->SetTowerPrice(TowerPriceMap[TowerNames[2]]);
	}
	if (CatapultButton)
	{
		CatapultButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCatapultButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[3]))
			CatapultButton->SetTowerPrice(TowerPriceMap[TowerNames[3]]);
	}
	
	return true;
}

void USTowerSelectionMenu::OnCannonButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(TowerNames[0])) return;
	SpawnGivenTower(CanonTowerClass);
}

void USTowerSelectionMenu::OnMachineGunButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(TowerNames[1])) return;
	SpawnGivenTower(MachineGunTowerClass);
}

void USTowerSelectionMenu::OnArcherTowerButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(TowerNames[2])) return;
	SpawnGivenTower(ArcherTowerClass);
}

void USTowerSelectionMenu::OnCatapultButtonClicked()
{
	if (!CheckAndDeductIfEnoughCoins(TowerNames[3])) return;
	SpawnGivenTower(CatapultTowerClass);
}

void USTowerSelectionMenu::SpawnGivenTower(const TSubclassOf<ASBaseTower>& TowerToSpawn)
{
	if (!IsValid(OwningTowerSite) || !TowerToSpawn->IsValidLowLevel()) return;

	FTransform SpawnTransform = OwningTowerSite->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation();
	SpawnLocation.Z += 60.f;
	SpawnTransform.SetLocation(SpawnLocation);

	ASBaseTower* SpawnedTower = GetWorld()->SpawnActor<ASBaseTower>(
		TowerToSpawn,
		SpawnTransform
	);

	SpawnedTower->SetTowerSite(OwningTowerSite);
	
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

bool USTowerSelectionMenu::CheckAndDeductIfEnoughCoins(const FName& InTowerName)
{
	uint32 InTowerPrice = 999;	
	if (TowerPriceMap.Contains(InTowerName)) InTowerPrice = TowerPriceMap[InTowerName];
	
	BaseGameMode = BaseGameMode.IsValid() ? BaseGameMode : Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!BaseGameMode.IsValid()) return false;
	if (BaseGameMode.IsValid() && BaseGameMode->DeductCoins(InTowerPrice)) return true;

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
