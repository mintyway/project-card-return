// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRManyCardItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRManyCardItem, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRManyCardItem : public APCRBaseItem
{
	GENERATED_BODY()

public:
	APCRManyCardItem();

protected:
	virtual void PlayerOrCardOverlapEvent() override;
};
