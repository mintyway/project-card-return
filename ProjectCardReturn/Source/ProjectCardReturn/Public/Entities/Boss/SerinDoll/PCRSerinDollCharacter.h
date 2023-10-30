// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollCharacter.generated.h"

class APCRLiftActor;
class APCREricaCharacter;
class APCRSerinDollHandBaseCharacter;
class APCRSerinDollLeftHandCharacter;
class APCRSerinDollRightHandCharacter;

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollCharacter : public APCRSerinDollBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollCharacter();

	friend class APCRSerinDollHandBaseCharacter;
	friend class APCRSerinDollLeftHandCharacter;
	friend class APCRSerinDollRightHandCharacter;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // Getter, Setter

private: // 내부 함수
	void SpawnHands();

private: // 내부 Getter
	FORCEINLINE float GetLiftHeight();
	FORCEINLINE float GetHandWorldHeight();
	
private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedErica;

	UPROPERTY()
	TObjectPtr<APCRLiftActor> CachedLift;

private: // 핸드
	UPROPERTY()
	TObjectPtr<APCRSerinDollLeftHandCharacter> LeftHand;

	UPROPERTY()
	TObjectPtr<APCRSerinDollRightHandCharacter> RightHand;

	static const float ContactDistance;
	static const float FloatingHandHeight;
	
	static const float BasicChaseYDistance;
};
