
#include "UFO/SUFO.h"

ASUFO::ASUFO()
{
	PrimaryActorTick.bCanEverTick = false;

	UFOMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOMesh"));
	SetRootComponent(UFOMesh);

	this->Tags.Add(FName(TEXT("UFO")));
}

void ASUFO::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageCallback);
}

void ASUFO::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;

	if (Health <= 0)
	{
		
	}
}

