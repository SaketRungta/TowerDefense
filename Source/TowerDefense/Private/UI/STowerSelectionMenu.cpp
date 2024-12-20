
#include "UI/STowerSelectionMenu.h"

#include "Components/Button.h"
#include "GameFramework/Pawn.h"
#include "GameMode/SBaseGameMode.h"
#include "Interactions/STowerSite.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/STowerDefensePawn.h"
#include "Towers/SBaseTower.h"
#include "Towers/STowerDataAsset.h"
#include "UI/SBaseHUD.h"
#include "UI/STowerSelectionMenuButton.h"

bool USTowerSelectionMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	for (const TObjectPtr<USTowerDataAsset> CurrentDataAsset : DataAssetOfAllTowers)
	{
		TowerPriceMap.Add(CurrentDataAsset->TowerName, CurrentDataAsset->TowerBuyingPrice);
		TowerIconMap.Add(CurrentDataAsset->TowerName, CurrentDataAsset->Icon);
	}
	
	if (CannonButton)
	{
		CannonButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCannonButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[0]))
		{
			CannonButton->SetTowerPrice(TowerPriceMap[TowerNames[0]]);
			CannonButton->SetTowerIcon(TowerIconMap[TowerNames[0]]);
		}
	}
	
	if (MachineGunButton)
	{
		MachineGunButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnMachineGunButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[1]))
		{
			MachineGunButton->SetTowerPrice(TowerPriceMap[TowerNames[1]]);
			MachineGunButton->SetTowerIcon(TowerIconMap[TowerNames[1]]);
		}
	}
	
	if (ArcherTowerButton)
	{
		ArcherTowerButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnArcherTowerButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[2]))
		{
			ArcherTowerButton->SetTowerPrice(TowerPriceMap[TowerNames[2]]);
			ArcherTowerButton->SetTowerIcon(TowerIconMap[TowerNames[2]]);
		}
	}
	
	if (CatapultButton)
	{
		CatapultButton->GetTowerButton()->OnClicked.AddDynamic(this, &ThisClass::OnCatapultButtonClicked);
		if (TowerPriceMap.Contains(TowerNames[3]))
		{
			CatapultButton->SetTowerPrice(TowerPriceMap[TowerNames[3]]);
			CatapultButton->SetTowerIcon(TowerIconMap[TowerNames[3]]);
		}
	}
	
	return true;
}

void USTowerSelectionMenu::NativeDestruct()
{
	if (PopInAnimation)
	{
		StopAnimation(PopInAnimation);
		UnbindAllFromAnimationFinished(PopInAnimation);
	}
	
	Super::NativeDestruct();
}

void USTowerSelectionMenu::FinishedPlayingPopInAnim() const
{
	if (OwningTowerSite.IsValid()) OwningTowerSite->HideTowerSelectionMenuWidgetComponent();
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
	if (!OwningTowerSite.IsValid() || !TowerToSpawn->IsValidLowLevel()) return;

	FTransform SpawnTransform = OwningTowerSite->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation();
	SpawnLocation.Z += 60.f;
	SpawnTransform.SetLocation(SpawnLocation);

	ASBaseTower* SpawnedTower = GetWorld()->SpawnActor<ASBaseTower>(
		TowerToSpawn,
		SpawnTransform
	);

	SpawnedTower->SetTowerSite(OwningTowerSite.Get());
	
	/** A tower has been spawned on the site so we need to disable it otherwise it will respond to player clicks and hovers */
	OwningTowerSite->SetIsSiteDisabled(true);
	
	PlayerPawn = PlayerPawn.IsValid() == true ? PlayerPawn : Cast<ASTowerDefensePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn.IsValid()) PlayerPawn->SetCurrentlySelectedTowerSite(nullptr);
}

bool USTowerSelectionMenu::CheckAndDeductIfEnoughCoins(const FName& InTowerName)
{
	uint32 InTowerPrice = 999;	
	if (TowerPriceMap.Contains(InTowerName)) InTowerPrice = TowerPriceMap[InTowerName];
	
	BaseGameMode = BaseGameMode.IsValid() ? BaseGameMode : Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode());
	if (BaseGameMode.IsValid() && BaseGameMode->DeductCoins(InTowerPrice)) return true;
	
	BaseHUD = BaseHUD.IsValid() ? BaseHUD : Cast<ASBaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(BaseHUD.IsValid()) BaseHUD->ShowMessageInHUD(FString("Not Enough Coins"), EMessageTypeToDisplay::Error);
	
	return false;
}

void USTowerSelectionMenu::PlayPopInAnimation(bool bIsReversed, const float& PlaybackSpeed)
{
	if (!PopInAnimation) return;

	UnbindAllFromAnimationFinished(PopInAnimation);

	FWidgetAnimationDynamicEvent WidgetAnimationDynamicEvent;
	WidgetAnimationDynamicEvent.BindUFunction(this, FName("FinishedPlayingPopInAnim"));
	
	if (bIsReversed)
	{
		PlayAnimationReverse(PopInAnimation, PlaybackSpeed);
		BindToAnimationFinished(PopInAnimation, WidgetAnimationDynamicEvent);
	}
	else PlayAnimationForward(PopInAnimation, PlaybackSpeed);
}

void USTowerSelectionMenu::SetOwningTowerSite(ASTowerSite* InOwningTowerSite)
{
	OwningTowerSite = InOwningTowerSite;
}
