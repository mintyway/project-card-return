// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "CMonsterBaseCharacter.generated.h"

UCLASS()
class PROJECTCARDRETURN_API ACMonsterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACMonsterBaseCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	


};
