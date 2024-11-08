
#include "UI/SHUDWidget.h"

#include "Components/TextBlock.h"
#include "GameMode/SBaseGameMode.h"
#include "Kismet/GameplayStatics.h"

bool USHUDWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (TB_LifeCount) TB_LifeCount->TextDelegate.BindUFunction(this, FName("BindLifeCount"));
	if (TB_CoinCount) TB_CoinCount->TextDelegate.BindUFunction(this, FName("BindCoinCount"));
	if (TB_WaveCount) TB_WaveCount->TextDelegate.BindUFunction(this, FName("BindWaveCount"));
	
	return true;
}

void USHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ASBaseGameMode>(UGameplayStatics::GetGameMode(this));
	if (IsValid(GameMode)) InitialLifeCount = GameMode->GetLifeCount();
}

FText USHUDWidget::BindLifeCount()
{
	if (IsValid(GameMode))
	{
		CurrentLifeCount = GameMode->GetLifeCount();
		return FText::FromString(FString::Printf(TEXT("%u"), CurrentLifeCount));
	}
	return FText::FromString(TEXT("Er"));
}

FText USHUDWidget::BindCoinCount() const
{
	if (IsValid(GameMode)) return FText::FromString(FString::Printf(TEXT("%u"), GameMode->GetCoinCount()));
	return FText::FromString(TEXT("Er"));
}

FText USHUDWidget::BindWaveCount() const
{
	if (IsValid(GameMode)) return FText::FromString(FString::Printf(TEXT("%u/%u"), GameMode->GetCurrentWaveNumber(), GameMode->GetTotalNumWaves()));
	return FText::FromString(TEXT("Er"));
}
