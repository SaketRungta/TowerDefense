
#include "UFO/SUFO.h"

#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "UFO/SUFOWaveManager.h"

ASUFO::ASUFO()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	UFOMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFOMesh"));
	UFOMesh->SetupAttachment(SceneRoot);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(SceneRoot);
	HealthBarWidget->SetHiddenInGame(true);
	HealthBarWidget->SetRelativeLocation(FVector(0, -80.f, 60.f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(75.f, 5.f));

	this->Tags.Add(FName(TEXT("UFO")));
}

void ASUFO::BeginDestroy()
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(this);
	}
	
	OnHideHealthWidget.Clear();
	OnResetIfAnimIsPlaying.Clear();
	OnUFODestroyed.Clear();
	OnUFOReachedBase.Clear();
	
	OnHideHealthWidget.RemoveAll(this);
	OnResetIfAnimIsPlaying.RemoveAll(this);
	OnUFODestroyed.RemoveAll(this);
	OnUFOReachedBase.RemoveAll(this);
	
	Super::BeginDestroy();
}

void ASUFO::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageCallback);
}

void ASUFO::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(HealthBarWidgetVisibilityTimerHandle);
		TimerManager.ClearAllTimersForObject(this);
	}

	OnHideHealthWidget.Clear();
	OnResetIfAnimIsPlaying.Clear();
	OnUFODestroyed.Clear();
	OnUFOReachedBase.Clear();
	
	OnHideHealthWidget.RemoveAll(this);
	OnResetIfAnimIsPlaying.RemoveAll(this);
	OnUFODestroyed.RemoveAll(this);
	OnUFOReachedBase.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

void ASUFO::OnUFOReachedBaseCall()
{
	OnUFOReachedBase.Broadcast(UFOLifeDamageCount);
	Destroy();
}

void ASUFO::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage>= CurrentHealth)
	{
		OnUFODestroyed.Broadcast(UFOCoinValue);
		if (WaveManager.IsValid()) WaveManager->SpawnedUFODestroyedByPlayerCallback(this);
		Destroy();
		return;
	}
	
	OnResetIfAnimIsPlaying.Broadcast();	
	
	HealthBarWidget->SetHiddenInGame(false);
	
	CurrentHealth -= Damage;
	HealthPercentage = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);

	GetWorldTimerManager().ClearTimer(HealthBarWidgetVisibilityTimerHandle);	
	GetWorldTimerManager().SetTimer(
		HealthBarWidgetVisibilityTimerHandle,
		[this]()
		{
			OnHideHealthWidget.Broadcast();
		},
		3,
		false
		);
}

void ASUFO::SetSplinePath(USplineComponent* InSplinePath)
{
	SplinePath = InSplinePath;
}

void ASUFO::SetWaveManager(ASUFOWaveManager* InWaveManager)
{
	WaveManager = InWaveManager;
}
