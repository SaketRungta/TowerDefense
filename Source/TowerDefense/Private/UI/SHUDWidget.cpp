
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
		BindToAnimationFinished(LastPlayedAnim.Get(), OnLastPlayedAnimFinishedPlaying);
}

void USHUDWidget::PlayStarsAnim()
{
	if (InitialLifeCount == CurrentLifeCount) StarsAwarded = 3;
	else if (CurrentLifeCount > InitialLifeCount / 2 ) StarsAwarded = 2;

	if (StarPopInAnimPlayedCount == StarsAwarded) return;

	switch (StarPopInAnimPlayedCount)
	{
	case 0:
		{
			PlayAnimationForward(PopFirstStarAnim);
		
			FWidgetAnimationDynamicEvent FinishedPlayingStarAnim;
			FinishedPlayingStarAnim.BindUFunction(this, FName("PlayStarsAnim"));
	
			BindToAnimationFinished(PopFirstStarAnim, FinishedPlayingStarAnim);
		}
		break;
	case 1:
		{			
			PlayAnimationForward(PopSecondStarAnim);
		
			FWidgetAnimationDynamicEvent FinishedPlayingStarAnim;
			FinishedPlayingStarAnim.BindUFunction(this, FName("PlayStarsAnim"));
	
			BindToAnimationFinished(PopSecondStarAnim, FinishedPlayingStarAnim);
		}
		break;
	case 2:
		PlayAnimationForward(PopThirdStarAnim);
		break;
	default:
		{			
			PlayAnimationForward(PopFirstStarAnim);
		
			FWidgetAnimationDynamicEvent FinishedPlayingStarAnim;
			FinishedPlayingStarAnim.BindUFunction(this, FName("PlayStarsAnim"));
	
			BindToAnimationFinished(PopFirstStarAnim, FinishedPlayingStarAnim);
		}
		break;
	}
	
	++StarPopInAnimPlayedCount;
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
