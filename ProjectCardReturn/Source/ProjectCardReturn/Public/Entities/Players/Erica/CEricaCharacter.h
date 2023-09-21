// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "CEricaCharacter.generated.h"

class ACEricaCardProjectile;
class ACEricaPlayerController;
class UCEricaDataAsset;
class UCameraComponent;
class USpringArmComponent;

/**
 * 캐릭터의 공격 모드를 설정합니다.
 */
enum class ShootMode
{
	Rapid,
	Buckshot
};

UCLASS()
class PROJECTCARDRETURN_API ACEricaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACEricaCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot();
	void Return();
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void RapidShoot();
	void BuckshotShoot();
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;

	TObjectPtr<UCEricaDataAsset> EricaDataAsset;
	TObjectPtr<ACEricaPlayerController> CachedEricaPlayerController;
	ShootMode CurrentShootMode;
	TArray<TObjectPtr<ACEricaCardProjectile>> CardProjectileQueue;
};
