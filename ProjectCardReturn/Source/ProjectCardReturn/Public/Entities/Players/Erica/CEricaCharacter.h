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
class ACEricaCardProjectilePool;
class UCGameInstance;
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
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void ShootCard();
	void ReturnCard();
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void RapidShot();
	void BuckShot();
	void Dash();
	void Change();

	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;
	
	TObjectPtr<ACEricaCardProjectilePool> CardPool;
	TObjectPtr<UCEricaDataAsset> EricaDataAsset;
	TObjectPtr<ACEricaPlayerController> CachedEricaPlayerController;
	TArray<TObjectPtr<ACEricaCardProjectile>> CardProjectileArray;
	ShootMode CurrentShootMode;
	bool bCanRapidShot;
	float RapidShotCoolTime;
	bool bCanBuckShot;
	float BuckShotCoolTime;
};
