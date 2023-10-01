// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCREricaCharacter.generated.h"

class UPCRParameterDataAsset;
class APCREricaCardProjectile;
class APCREricaPlayerController;
class UPCREricaDataAsset;
class UCameraComponent;
class USpringArmComponent;
class APCREricaCardProjectilePool;
class UPCRGameInstance;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCharacter, Log, All);

/**
 * 캐릭터의 공격 모드를 설정합니다.
 */
enum class ShootMode
{
	Rapid,
	Buckshot
};

UCLASS()
class PROJECTCARDRETURN_API APCREricaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCREricaCharacter();

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
	
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
	TObjectPtr<APCREricaCardProjectilePool> CardPool;
	TObjectPtr<APCREricaPlayerController> CachedEricaPlayerController;
	TArray<TObjectPtr<APCREricaCardProjectile>> CardProjectileArray;

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
