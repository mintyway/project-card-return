// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/Serin/Hand/Base/PCRSerinHandBaseCharacter.h"
#include "PCRSerinLeftHandCharacter.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRSerinLeftHandCharacter : public APCRSerinHandBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinLeftHandCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // 내부 함수
	virtual void HandleChase(float DeltaTime) override;
};
