// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "CEricaRossCharacter.generated.h"

class UCEricaRossDataAsset;
class UCameraComponent;
class USpringArmComponent;

/**
 * 캐릭터의 공격 모드를 설정합니다.
 */
enum class ShootMode
{
	Auto,
	
};

UCLASS()
class PROJECTCARDRETURN_API ACEricaRossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACEricaRossCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot();
	
private:
	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;

	TObjectPtr<UCEricaRossDataAsset> DataAsset;
	
	ShootMode CurrentShootMode;
};
