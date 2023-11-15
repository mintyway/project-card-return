// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRHealItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRHealItem, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRHealItem : public APCRBaseItem
{
	GENERATED_BODY()

public:
	APCRHealItem();

protected:
	virtual void PlayerOverlapEvent() override;
};
