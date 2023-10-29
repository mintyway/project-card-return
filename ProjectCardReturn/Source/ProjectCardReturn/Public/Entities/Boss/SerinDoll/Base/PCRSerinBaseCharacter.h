// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCRSerinBaseCharacter.generated.h"

class UPCRSerinPrimaryDataAsset;
class UPCRParameterDataAsset;

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCRSerinBaseCharacter();


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRSerinPrimaryDataAsset> SerinDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
};
