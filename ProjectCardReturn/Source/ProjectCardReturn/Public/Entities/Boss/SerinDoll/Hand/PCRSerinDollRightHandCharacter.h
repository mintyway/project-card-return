// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Hand/Base/PCRSerinDollHandBaseCharacter.h"
#include "PCRSerinDollRightHandCharacter.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollRightHandCharacter : public APCRSerinDollHandBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollRightHandCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // 내부 함수
	virtual void PaperChase(bool bUseReset) override;
	virtual void HandleBasicChase(float DeltaTime) override;
	virtual void HandlePaperChase(float DeltaTime) override;
};
