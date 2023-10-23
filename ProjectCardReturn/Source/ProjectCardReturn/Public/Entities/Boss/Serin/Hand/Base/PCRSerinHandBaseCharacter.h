// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Boss/Serin/Base/PCRSerinBaseCharacter.h"
#include "PCRSerinHandBaseCharacter.generated.h"

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinHandBaseCharacter : public APCRSerinBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinHandBaseCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// TODO: 더미 스켈레탈 메시 구현시 제거
	UPROPERTY(VisibleAnywhere, Category = "DummyMeshComponent")
	TObjectPtr<UStaticMeshComponent> DummyStaticMeshComponent;
};
