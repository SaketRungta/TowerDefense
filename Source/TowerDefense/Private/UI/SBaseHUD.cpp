
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

void ASBaseHUD::ShowMessageInHUD(const FString& InMessage, EMessageTypeToDisplay InMessageType) const
{
	if (HUDWidget.IsValid()) HUDWidget->ShowMessageInHUD(InMessage, InMessageType);
}

void ASBaseHUD::ShowTheGivenMenu(const EMenuToShow InMenuToShow) const
{
	if (HUDWidget.IsValid()) HUDWidget->ShowTheGivenMenu(InMenuToShow);
}
