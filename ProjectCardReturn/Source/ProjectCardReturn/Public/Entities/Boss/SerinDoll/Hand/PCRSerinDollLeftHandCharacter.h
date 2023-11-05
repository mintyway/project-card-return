// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Hand/Base/PCRSerinDollHandBaseCharacter.h"
#include "PCRSerinDollLeftHandCharacter.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollLeftHandCharacter : public APCRSerinDollHandBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollLeftHandCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // 내부 함수
	virtual void PaperChase(bool bUseReset) override;
	virtual void HandleBasicChase(float DeltaTime) override;
	virtual void HandlePaperChase(float DeltaTime) override;
};
