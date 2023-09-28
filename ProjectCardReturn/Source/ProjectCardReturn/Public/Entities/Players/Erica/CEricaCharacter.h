// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "CEricaCharacter.generated.h"

class UCParameterDataAsset;
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
	float GetAttackPower() const { return AttackPower; }

	void ShootCard();
	void ReturnCard();
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void Dash();
	void HandleDash(float DeltaTime);
	
	void RapidShot();
	void BuckShot();
	void Change();
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
	
	TObjectPtr<const UCEricaDataAsset> EricaDataAsset;
	TObjectPtr<const UCParameterDataAsset> ParameterDataAsset;
	TObjectPtr<ACEricaCardProjectilePool> CardPool;
	TObjectPtr<ACEricaPlayerController> CachedEricaPlayerController;
	TArray<TObjectPtr<ACEricaCardProjectile>> CardProjectileArray;

	float AttackPower;

	TArray<FKey> MovementKeys;
	FVector LastInputMoveDirection;

	uint8 bCanDash:1;
	uint8 bIsDashing:1;
	uint8 bCanRapidShot:1;
	uint8 bCanBuckShot:1;

	float DashCoolTime;
	float TotalDashTime;
	float ElapsedDashTime;
	float DashDistance;
	FVector CachedDashStartLocation;
	FVector CachedDashDirection;
	
	ShootMode CurrentShootMode;
	float RapidShotCoolTime;
	float BuckShotCoolTime;
	
};
