// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRMoreHpItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRMoreHpItem : public APCRBaseItem
{
	GENERATED_BODY()

public:
	APCRMoreHpItem();

protected:
	virtual void PlayerOverlapEvent() override;
};
