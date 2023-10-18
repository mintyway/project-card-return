// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCardReturn.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PCRSoldierDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTService_PCRSoldierDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_PCRSoldierDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
