
#include "UI/SBaseHUD.h"

#include "ShaderPrintParameters.h"
#include "UI/SHUDWidget.h"

void ASBaseHUD::ShowErrorMessage(const FString& InErrorMessage)
{
	ISGameInteractionInterface::ShowErrorMessage(InErrorMessage);
	
	if (HUDWidget.IsValid())
	{
		HUDWidget->ShowErrorMessage(InErrorMessage);
	}
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
