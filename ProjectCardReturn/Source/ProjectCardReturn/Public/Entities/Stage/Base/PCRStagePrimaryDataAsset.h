// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PCRStagePrimaryDataAsset.generated.h"

class APCRLiftActor;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRStagePrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stage3")
	TSoftClassPtr<APCRLiftActor> LiftBlueprint;
};
