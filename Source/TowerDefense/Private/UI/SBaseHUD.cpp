
#include "UI/SBaseHUD.h"

#include "UI/SHUDWidget.h"

void ASBaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass))
	{
		HUDWidget = CreateWidget<USHUDWidget>(GetWorld()->GetFirstPlayerController(), HUDWidgetClass);
		HUDWidget->AddToViewport();
	}
}

void ASBaseHUD::ShowErrorMessage(const FString& InErrorMessage) const
{
	if (IsValid(HUDWidget)) HUDWidget->ShowErrorMessage(InErrorMessage);
}

void ASBaseHUD::ShowTheGivenMenu(const EMenuToShow InMenuToShow) const
{
	if (IsValid(HUDWidget)) HUDWidget->ShowTheGivenMenu(InMenuToShow);
}
