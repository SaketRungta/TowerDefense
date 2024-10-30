
#include "UI/SHUDWidget.h"

#include <string>

#include "Components/TextBlock.h"
#include "GameMode/SBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

bool USHUDWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	LifeCount->TextDelegate.BindUFunction(this, "BindLifeCount");
	
	return true;
}

void USHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ASBaseGameMode>(UGameplayStatics::GetGameMode(this));
}

FText USHUDWidget::BindLifeCount() const
{	
	if(GameMode.IsValid()) return FText::FromString(FString::Printf(TEXT("%u"), GameMode->GetLifeCount()));
	return FText::FromString(TEXT("Er"));	
}
