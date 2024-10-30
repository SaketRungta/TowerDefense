
#include "UI/SHUDWidget.h"
#include "Components/TextBlock.h"
#include "GameMode/SBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

bool USHUDWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	LifeCount->TextDelegate.BindUFunction(this, "BindLifeCount");
	CoinCount->TextDelegate.BindUFunction(this, "BindCoinCount");
	WaveCount->TextDelegate.BindUFunction(this, "BindWaveCount");
	
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

FText USHUDWidget::BindCoinCount() const
{
	if(GameMode.IsValid()) return FText::FromString(FString::Printf(TEXT("%u"), GameMode->GetCoinCount()));
	return FText::FromString(TEXT("Er"));
}

FText USHUDWidget::BindWaveCount() const
{
	if(GameMode.IsValid()) return FText::FromString(FString::Printf(TEXT("%u/%u"), GameMode->GetCurrentWaveNumber(), GameMode->GetTotalNumWaves()));
	return FText::FromString(TEXT("Er"));
}
