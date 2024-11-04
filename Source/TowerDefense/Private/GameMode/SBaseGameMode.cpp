
#include "GameMode/SBaseGameMode.h"
#include "Interface/SGameInteractionInterface.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

void ASBaseGameMode::OnUFOReachedBaseCallback(uint32 InUFOLifeCount)
{
	LifeCount -= InUFOLifeCount;

	if (LifeCount > 0) return;
	
	UGameplayStatics::SetGamePaused(this, true);

	AHUD* BaseHUD = GetWorld()->GetFirstPlayerController()->GetHUD();
	if (IsValid(BaseHUD))
	{
		if (BaseHUD->GetClass()->ImplementsInterface(USGameInteractionInterface::StaticClass()))
		{
			ISGameInteractionInterface* GameInteractionInterface = Cast<ISGameInteractionInterface>(BaseHUD);
			if (GameInteractionInterface != nullptr) GameInteractionInterface->ShowTheGivenMenu(EMenuToShow::TryAgain);
		}
	}

	APawn* BasePawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (IsValid(BasePawn))
	{
		if (BasePawn->GetClass()->ImplementsInterface(USGameInteractionInterface::StaticClass()))
		{
			ISGameInteractionInterface* GameInteractionInterface = Cast<ISGameInteractionInterface>(BasePawn);
			if (GameInteractionInterface != nullptr) GameInteractionInterface->SetCanPauseTheGame(false);
		}
	}
}
