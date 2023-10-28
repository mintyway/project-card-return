// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/Serin/Base/PCRSerinBaseCharacter.h"
#include "PCRSerinCharacter.generated.h"

class APCREricaCharacter;
class APCRSerinHandBaseCharacter;
class APCRSerinLeftHandCharacter;
class APCRSerinRightHandCharacter;

UCLASS()
class PROJECTCARDRETURN_API APCRSerinCharacter : public APCRSerinBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinCharacter();

	friend class APCRSerinHandBaseCharacter;
	friend class APCRSerinLeftHandCharacter;
	friend class APCRSerinRightHandCharacter;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // Getter, Setter

private: // 내부 함수
	void SpawnHands();

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Dummy")
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;

private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedErica;

private: // 핸드
	UPROPERTY()
	TObjectPtr<APCRSerinLeftHandCharacter> LeftHand;

	UPROPERTY()
	TObjectPtr<APCRSerinRightHandCharacter> RightHand;

	FVector LeftHandChaseDistance;
	FVector RightHandChaseDistance;
};
