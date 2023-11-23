// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Item/PCRBaseItem.h"
#include "PCRLongerRangeItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRLongerRangeItem, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRLongerRangeItem : public APCRBaseItem
{
	GENERATED_BODY()

public:
	APCRLongerRangeItem();

protected:
	virtual void PlayerOrCardOverlapEvent() override;
};
