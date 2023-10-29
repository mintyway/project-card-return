// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCRSerinDollBaseCharacter.generated.h"

class UPCRSerinDollPrimaryDataAsset;
class UPCRParameterDataAsset;

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinDollBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollBaseCharacter();


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRSerinDollPrimaryDataAsset> SerinDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
};
