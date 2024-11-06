
#include "UI/STowerSelectionMenuButton.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTowerSelectionMenuButton::SetTowerPrice(const int32 InTowerPrice) const
{
	if (T_TowerPrice) T_TowerPrice->SetText(FText::FromString(FString::FromInt(InTowerPrice)));
}

void USTowerSelectionMenuButton::SetTowerIcon(const FSlateBrush& InTowerIcon) const
{
	if (TowerIcon) TowerIcon->SetBrush(InTowerIcon);
}
