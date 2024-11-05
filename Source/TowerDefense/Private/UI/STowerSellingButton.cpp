
#include "UI/STowerSellingButton.h"
#include "Components/Button.h"
#include "Towers/SBaseTower.h"
#include "Components/TextBlock.h"
#include "GameMode/SBaseGameMode.h"

bool USTowerSellingButton::Initialize()
{
	if (!Super::Initialize()) return false;

	if (SellButton) SellButton->OnClicked.AddDynamic(this, &ThisClass::OnSellButtonClicked);
	
	return true;
}

void USTowerSellingButton::OnSellButtonClicked()
{
	if (ASBaseGameMode* BaseGameMode = Cast<ASBaseGameMode>(GetWorld()->GetAuthGameMode()))
		BaseGameMode->AddCoins(TowerSellingPrice);
	
	if (OwningTower.IsValid()) OwningTower->Destroy();
}

void USTowerSellingButton::SetOwningTower(ASBaseTower* InOwningTower)
{
	OwningTower = InOwningTower;
}

void USTowerSellingButton::SetTowerSellingPrice(const uint32& InTowerSellingPrice)
{
	TowerSellingPrice = InTowerSellingPrice;
	if (T_TowerSellingPrice) T_TowerSellingPrice->SetText(FText::FromString(FString::Printf(TEXT("%d"), InTowerSellingPrice)));
}
