
#include "UFO/SUFO.h"

ASUFO::ASUFO()
{
	PrimaryActorTick.bCanEverTick = false;

	UFOMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOMesh"));
	SetRootComponent(UFOMesh);
}

void ASUFO::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASUFO::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

