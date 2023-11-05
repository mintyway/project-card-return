// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCRSerinDollBaseCharacter.generated.h"

class UPCRSoundPrimaryDataAsset;
class UPCRSerinDollPrimaryDataAsset;
class UPCRParameterDataAsset;

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinDollBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollBaseCharacter();
	
protected:
	virtual void PostInitializeComponents() override;

protected: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;
};
