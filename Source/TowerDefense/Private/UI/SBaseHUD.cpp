
#include "UI/SBaseHUD.h"
#include "UI/SHUDWidget.h"

void ASBaseHUD::ShowErrorMessage(const FString& InErrorMessage)
{
	ISGameInteractionInterface::ShowErrorMessage(InErrorMessage);
	
	if (HUDWidget.IsValid()) HUDWidget->ShowErrorMessage(InErrorMessage);
}

void ASBaseHUD::ShowTheGivenMenu(EMenuToShow InMenuToShow)
{
	ISGameInteractionInterface::ShowTheGivenMenu(InMenuToShow);

	if (HUDWidget.IsValid()) HUDWidget->ShowTheGivenMenu(InMenuToShow);
}

void ASBaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass))
	{
		HUDWidget = CreateWidget<USHUDWidget>(GetWorld()->GetFirstPlayerController(), HUDWidgetClass);
		HUDWidget->AddToViewport();
	}
}
