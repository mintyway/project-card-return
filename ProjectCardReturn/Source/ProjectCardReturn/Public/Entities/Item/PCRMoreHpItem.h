// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRMoreHpItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRMoreHpItem, Log, All);

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
	virtual void PlayerOrCardOverlapEvent() override;
};
