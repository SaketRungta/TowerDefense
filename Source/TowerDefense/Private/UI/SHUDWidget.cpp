
#include "UI/SHUDWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
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

	BaseGameMode = Cast<ASBaseGameMode>(UGameplayStatics::GetGameMode(this));
	if (BaseGameMode.IsValid()) InitialLifeCount = BaseGameMode->GetLifeCount();
}

void USHUDWidget::NativeDestruct()
{
	if (HUDPopOutAnim)
	{
		StopAnimation(HUDPopOutAnim);
		UnbindAllFromAnimationFinished(HUDPopOutAnim);
	}
	if (ShowLevelCompletedMenuAnim)
	{
		StopAnimation(ShowLevelCompletedMenuAnim);
		UnbindAllFromAnimationFinished(ShowLevelCompletedMenuAnim);
	}
	if (ShowTryAgainMenuAnim)
	{
		StopAnimation(ShowTryAgainMenuAnim);
		UnbindAllFromAnimationFinished(ShowTryAgainMenuAnim);
	}
	if (ShowPausedMenuAnim)
	{
		StopAnimation(ShowPausedMenuAnim);
		UnbindAllFromAnimationFinished(ShowPausedMenuAnim);
	}
	if (PopFirstStarAnim)
	{
		StopAnimation(PopFirstStarAnim);
		UnbindAllFromAnimationFinished(PopFirstStarAnim);
	}
	if (PopSecondStarAnim)
	{
		StopAnimation(PopSecondStarAnim);
		UnbindAllFromAnimationFinished(PopSecondStarAnim);
	}
	if (PopThirdStarAnim)
	{
		StopAnimation(PopThirdStarAnim);
		UnbindAllFromAnimationFinished(PopThirdStarAnim);
	}
	
	FinishedPlayingStarAnim.Unbind();

	Super::NativeDestruct();
}

FText USHUDWidget::BindLifeCount()
{
	if (BaseGameMode.IsValid())
	{
		CurrentLifeCount = BaseGameMode->GetLifeCount();
		return FText::FromString(FString::Printf(TEXT("%u"), CurrentLifeCount));
	}
	return FText::FromString(TEXT("Er"));
}

FText USHUDWidget::BindCoinCount() const
{
	if (BaseGameMode.IsValid()) return FText::FromString(FString::Printf(TEXT("%u"), BaseGameMode->GetCoinCount()));
	return FText::FromString(TEXT("Er"));
}

FText USHUDWidget::BindWaveCount() const
{
	if (BaseGameMode.IsValid()) return FText::FromString(FString::Printf(TEXT("%u/%u"), BaseGameMode->GetCurrentWaveNumber(), BaseGameMode->GetTotalNumWaves()));
	return FText::FromString(TEXT("Er"));
}

void USHUDWidget::PopOutHUD(const int32& InWidgetIndex, UWidgetAnimation* InAnimToPlay)
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	if (LastPlayedAnim.IsValid()) UnbindAllFromAnimationFinished(LastPlayedAnim.Get());
	
	UnbindAllFromAnimationFinished(HUDPopOutAnim);

	PlayAnimationForward(HUDPopOutAnim);

	FWidgetAnimationDynamicEvent OnHUDPopOutAnimFinishedPlaying;
	OnHUDPopOutAnimFinishedPlaying.BindUFunction(this, FName("ShowTheRequestedMenu"));
	
	WidgetIndexToActivate = InWidgetIndex;
	LastPlayedAnim = InAnimToPlay;
	
	BindToAnimationFinished(HUDPopOutAnim, OnHUDPopOutAnimFinishedPlaying);
}

void USHUDWidget::ShowTheRequestedMenu()
{
	if (!LastPlayedAnim.IsValid()) return;
	UnbindAllFromAnimationFinished(LastPlayedAnim.Get());
	
	WidgetSwitcher->SetActiveWidgetIndex(WidgetIndexToActivate);
	if (LastPlayedAnim.IsValid()) PlayAnimationForward(LastPlayedAnim.Get());

	FWidgetAnimationDynamicEvent OnLastPlayedAnimFinishedPlaying;
	OnLastPlayedAnimFinishedPlaying.BindUFunction(this, FName("PlayStarsAnim"));
	
	if (LastPlayedAnim == ShowLevelCompletedMenuAnim)
	{
		if (InitialLifeCount == CurrentLifeCount) StarsAwarded = 3;
		else if (CurrentLifeCount > InitialLifeCount / 2 ) StarsAwarded = 2;

		SetLevelStarsData();

		BindToAnimationFinished(LastPlayedAnim.Get(), OnLastPlayedAnimFinishedPlaying);
	}
}

void USHUDWidget::PlayStarsAnim()
{
	if (!IsValid(this)) return;
	
	FinishedPlayingStarAnim.Unbind();
	FinishedPlayingStarAnim.BindUFunction(this, FName("PlayStarsAnim"));
	
	if (StarPopInAnimPlayedCount == StarsAwarded) return;

	switch (StarPopInAnimPlayedCount)
	{
	case 0:
		PlayAnimationForward(PopFirstStarAnim);
		BindToAnimationFinished(PopFirstStarAnim, FinishedPlayingStarAnim);
		break;
	case 1:
		PlayAnimationForward(PopSecondStarAnim);
		BindToAnimationFinished(PopSecondStarAnim, FinishedPlayingStarAnim);
		break;
	case 2:
		PlayAnimationForward(PopThirdStarAnim);
		break;
	default:
		PlayAnimationForward(PopFirstStarAnim);
		BindToAnimationFinished(PopFirstStarAnim, FinishedPlayingStarAnim);
		break;
	}
	
	++StarPopInAnimPlayedCount;
}

void USHUDWidget::SetLevelStarsData()
{
	const FString CurrLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (CurrLevelName == "lv_01") SaveLevelStarsData(0, StarsAwarded);
	else if (CurrLevelName == "lv_02") SaveLevelStarsData(1, StarsAwarded);
	else if (CurrLevelName == "lv_03") SaveLevelStarsData(2, StarsAwarded);
	else if (CurrLevelName == "lv_04") SaveLevelStarsData(3, StarsAwarded);
	else if (CurrLevelName == "lv_05") SaveLevelStarsData(4, StarsAwarded);
	else if (CurrLevelName == "lv_06") SaveLevelStarsData(5, StarsAwarded);
	else if (CurrLevelName == "lv_07") SaveLevelStarsData(6, StarsAwarded);
	else if (CurrLevelName == "lv_08") SaveLevelStarsData(7, StarsAwarded);
	else if (CurrLevelName == "lv_09") SaveLevelStarsData(8, StarsAwarded);
	else if (CurrLevelName == "lv_10") SaveLevelStarsData(9, StarsAwarded);
}

void USHUDWidget::PopInHUD()
{
	if (!LastPlayedAnim.IsValid()) return;
	UnbindAllFromAnimationFinished(LastPlayedAnim.Get());

	UnbindAllFromAnimationFinished(HUDPopOutAnim);

	PlayAnimationReverse(LastPlayedAnim.Get());
	
	FWidgetAnimationDynamicEvent OnLastPlayedAnimFinishedPlaying;
	OnLastPlayedAnimFinishedPlaying.BindUFunction(this, FName("ShowTheMainHUD"));
	
	BindToAnimationFinished(LastPlayedAnim.Get(), OnLastPlayedAnimFinishedPlaying);
}

void USHUDWidget::ShowTheMainHUD()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	if (HUDPopOutAnim) PlayAnimationReverse(HUDPopOutAnim);
}

void USHUDWidget::ShowTheGivenMenu(const EMenuToShow InMenuToShow)
{
	switch (InMenuToShow)
	{
	case EMenuToShow::None:
		PopInHUD();
		break;
	case EMenuToShow::LevelCompleted:
		PopOutHUD(1, ShowLevelCompletedMenuAnim);
		break;
	case EMenuToShow::TryAgain:
		PopOutHUD(2, ShowTryAgainMenuAnim);
		break;
	case EMenuToShow::Pause:
		PopOutHUD(3, ShowPausedMenuAnim);
		break;
	case EMenuToShow::HUD:
		PopInHUD();
		break;
	}
}
