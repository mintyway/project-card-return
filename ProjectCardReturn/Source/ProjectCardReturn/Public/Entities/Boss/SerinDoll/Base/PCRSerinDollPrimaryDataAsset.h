// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRSerinDollPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SerinDoll")
	TObjectPtr<USkeletalMesh> SerinDollMesh;

	UPROPERTY(EditAnywhere, Category = "SerinDollHands")
	TObjectPtr<USkeletalMesh> SerinDollLeftHandMesh;

	UPROPERTY(EditAnywhere, Category = "SerinDollHands")
	TObjectPtr<USkeletalMesh> SerinDollRightHandMesh;
};
