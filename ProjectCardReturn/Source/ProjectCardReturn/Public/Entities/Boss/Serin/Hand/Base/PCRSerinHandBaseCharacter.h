// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Boss/Serin/Base/PCRSerinBaseCharacter.h"
#include "PCRSerinHandBaseCharacter.generated.h"

class APCRSerinCharacter;
class APCRSerinHandBaseCharacter;
class APCRSerinLeftHandCharacter;
class APCRSerinRightHandCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinHandBaseCharacter, Log, All);
DECLARE_DELEGATE(FChaseEndSignature);

UENUM()
enum class ESerinState
{
	Idle,
	Move,
	Chase,
	BasicChase,
	Rock,
	Paper,
	Scissors,
	Invalid
};

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinHandBaseCharacter : public APCRSerinBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinHandBaseCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // 동작
	void SetTarget(AActor* TargetActor);
	void Move(const FVector& InLocation);
	void BasicChase();
	void Chase();
	void Rock();

public:
	FChaseEndSignature OnChaseEnd;
	
protected: // 내부 함수
	void HandleMove(float DeltaTime);
	virtual void HandleBasicChase(float DeltaTime);
	void HandleChase(float DeltaTime);
	void RockCallback();
	void HandleRock(float DeltaTime);

protected:
	// TODO: 더미 스켈레탈 메시 구현시 제거
	UPROPERTY(VisibleAnywhere, Category = "DummyMeshComponent")
	TObjectPtr<UStaticMeshComponent> DummyStaticMeshComponent;

protected: // 캐시
	UPROPERTY()
	TObjectPtr<APCRSerinCharacter> CachedSerinCharacter;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget; 

protected: // 이동
	FVector MoveLocation;
	FVector TargetLocation;

protected: // 공격 패턴
	ESerinState CurrentSerinState;
};
