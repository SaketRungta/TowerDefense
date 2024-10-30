
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHUDWidget.generated.h"

class UTextBlock;
class ASBaseGameMode;

UCLASS(Blueprintable, BlueprintType, meta = (Category = "UI", DisableNativeTick))
class TOWERDEFENSE_API USHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Used to initialize widget components with bindings */
	virtual bool Initialize() override;

protected:
	virtual void NativeConstruct() override;
	
private:
	/**  */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> LifeCount;

	/**  */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CoinCount;

	/**  */
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> WaveCount;

	UFUNCTION()
	FText BindLifeCount() const;

	TWeakObjectPtr<ASBaseGameMode> GameMode;
};
