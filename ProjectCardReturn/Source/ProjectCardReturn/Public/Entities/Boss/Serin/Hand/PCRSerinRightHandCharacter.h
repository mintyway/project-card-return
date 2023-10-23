// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Base/PCRSerinHandBaseCharacter.h"
#include "PCRSerinRightHandCharacter.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRSerinRightHandCharacter : public APCRSerinHandBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinRightHandCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
