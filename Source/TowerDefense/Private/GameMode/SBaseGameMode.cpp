
#include "GameMode/SBaseGameMode.h"

void ASBaseGameMode::OnUFOReachedBaseCallback(uint32 InUFOLifeCount)
{
	LifeCount -= InUFOLifeCount;

	if (LifeCount <= 0)
	{
		// 
	}
}
