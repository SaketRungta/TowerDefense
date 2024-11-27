
#include "GameMode/SBaseGameMode.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/STowerDefensePawn.h"
#include "System/CommonTypes.h"
#include "UI/SBaseHUD.h"

void ASBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(this);
	
	const FString Prefix = TEXT("lv_");
	if (CurrentMapName.StartsWith(Prefix))
	{
		const FString LevelIndexString = CurrentMapName.RightChop(Prefix.Len());
		const int32 LevelIndex = FCString::Atoi(*LevelIndexString);
		CoinCount = InitialCoinsForEachLevel[LevelIndex];
	}
}

void ASBaseGameMode::OnUFOReachedBaseCallback(uint32 InUFOLifeCount)
{
	LifeCount -= InUFOLifeCount;

	if (LifeCount > 0) return;

	LifeCount = 0;
	
	UGameplayStatics::SetGamePaused(this, true);

	if (const ASBaseHUD* BaseHUD = Cast<ASBaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())) BaseHUD->ShowTheGivenMenu(EMenuToShow::TryAgain);

	if (ASTowerDefensePawn* BasePawn = Cast<ASTowerDefensePawn>(GetWorld()->GetFirstPlayerController()->GetPawn())) BasePawn->SetCanPauseTheGame(false);
}
