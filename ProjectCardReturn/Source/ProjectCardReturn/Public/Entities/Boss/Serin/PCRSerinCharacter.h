// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/Serin/Base/PCRSerinBaseCharacter.h"
#include "PCRSerinCharacter.generated.h"

class APCRSerinLeftHandCharacter;
class APCRSerinRightHandCharacter;

UCLASS()
class PROJECTCARDRETURN_API APCRSerinCharacter : public APCRSerinBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private: // 컴포넌트 섹션
	UPROPERTY(VisibleAnywhere, Category = "Dummy")
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;
	
private: // 소유 액터 섹션
	UPROPERTY()
	TObjectPtr<APCRSerinLeftHandCharacter> LeftHand;

	UPROPERTY()
	TObjectPtr<APCRSerinRightHandCharacter> RightHand;
};
