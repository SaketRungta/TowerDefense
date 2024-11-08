
#include "GameMode/SBaseGameMode.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/STowerDefensePawn.h"
#include "System/CommonTypes.h"
#include "UI/SBaseHUD.h"

void ASBaseGameMode::OnUFOReachedBaseCallback(uint32 InUFOLifeCount)
{
	LifeCount -= InUFOLifeCount;

	if (LifeCount > 0) return;
	
	UGameplayStatics::SetGamePaused(this, true);

	if (const ASBaseHUD* BaseHUD = Cast<ASBaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())) BaseHUD->ShowTheGivenMenu(EMenuToShow::TryAgain);

	if (ASTowerDefensePawn* BasePawn = Cast<ASTowerDefensePawn>(GetWorld()->GetFirstPlayerController()->GetPawn())) BasePawn->SetCanPauseTheGame(false);
}
