// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRStrongAttackItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRStrongAttackItem, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRStrongAttackItem : public APCRBaseItem
{
	GENERATED_BODY()

public:
	APCRStrongAttackItem();

protected:
	virtual void PlayerOverlapEvent() override;
};
