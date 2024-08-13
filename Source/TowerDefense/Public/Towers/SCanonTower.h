// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Towers/SBaseTower.h"
#include "SCanonTower.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ASCanonTower : public ASBaseTower
{
	GENERATED_BODY()
	
protected:

	virtual void FireTurret() override;

};
