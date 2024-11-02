
#include "UFO/SUFO.h"
#include "Components/WidgetComponent.h"

ASUFO::ASUFO()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	UFOMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOMesh"));
	UFOMesh->SetupAttachment(SceneRoot);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(SceneRoot);
	HealthBarWidget->SetRelativeLocation(FVector(0, -80.f, 60.f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(75.f, 5.f));

	CurrentHealth = MaxHealth;
	
	this->Tags.Add(FName(TEXT("UFO")));
}

void ASUFO::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageCallback);
}

void ASUFO::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	HealthPercentage = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);

	if (CurrentHealth <= 0) Destroy();
}
