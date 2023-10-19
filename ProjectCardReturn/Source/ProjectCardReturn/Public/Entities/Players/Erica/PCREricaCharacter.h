// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCREricaCharacter.generated.h"

class UPCREricaAnimInstance;
class UPCRParameterDataAsset;
class APCREricaCardProjectile;
class APCREricaPlayerController;
class UPCREricaDataAsset;
class UCameraComponent;
class USpringArmComponent;
class APCREricaCardProjectilePool;
class UPCRGameInstance;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FAttackSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeHPSignature, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeCardCountSignature, int32, int32);
DECLARE_MULTICAST_DELEGATE(FDeadSignature);

/**
 * 캐릭터의 공격 모드를 설정합니다.
 */
enum class ShootMode
{
	Normal,
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
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }
	
	FORCEINLINE float GetNormalShotForwardDamage() const { return NormalShotForwardDamage; }
	FORCEINLINE float GetNormalShotBackwardDamage() const { return NormalShotBackwardDamage; }
	FORCEINLINE float GetBuckShotForwardDamage() const { return BuckShotForwardDamage; }
	FORCEINLINE float GetBuckShotBackwardDamage() const { return BuckShotBackwardDamage; }
	
	FORCEINLINE int32 GetMaxCardCount() const { return MaxCardCount; }
	FORCEINLINE int32 GetCurrentCardCount() const { return CurrentCardCount; }

	bool GetIsDashing() const { return bIsDashing; }

	void ShootCard();
	void ReturnCard();

	FAttackSignature OnCardShoot;
	FAttackSignature OnCardReturn;
	
	FChangeHPSignature OnChangeHP;
	FDeadSignature OnDead;

	FChangeCardCountSignature OnChangeCardCount;

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void Dash();
	void HandleDash(float DeltaTime);

	void HandleShootMode();
	void NormalShot();
	void BuckShot();
	void HandleShootCard(const FVector& Direction, float Range);
	void Change();

	void ReturnCardCooldownTimerCallback();
	void ShotCooldownTimerCallback();
	void DashCooldownTimerCallback();
	void TotalDashTimeCallback();

	void ChangeHP(float Amount);
	void HandleChangeHP();
	void HandleDead();

	void HandleChangeCardCount();
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Aim")
	TObjectPtr<UStaticMeshComponent> AimingPlane;

	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<APCREricaCardProjectilePool> CardPool;

	UPROPERTY()
	TObjectPtr<APCREricaPlayerController> CachedEricaPlayerController;
	
	UPROPERTY()
	TObjectPtr<UPCREricaAnimInstance> CachedEricaAnimInstance;

	TArray<TObjectPtr<APCREricaCardProjectile>> CardProjectiles;

	float MaxHP;
	float CurrentHP;
	uint32 bIsAlive:1;
	
	int32 BuckShotCount;
	float BuckShotAngle;
	
	float NormalShotForwardDamage;
	float NormalShotBackwardDamage;
	float BuckShotForwardDamage;
	float BuckShotBackwardDamage;

	int32 MaxCardCount;
	int32 CurrentCardCount;

	TArray<FKey> MovementKeys;
	FVector LastInputMoveDirection;

	uint32 bCanDash:1;
	uint32 bIsDashing:1;
	uint32 bCanRapidShot:1;
	uint32 bCanBuckShot:1;
	uint32 bCanReturnCard:1;

	float DashCooldownTime;
	float MaxDashTime;
	float ElapsedDashTime;
	float DashDistance;
	float ReturnCardCoolTime;
	FVector CachedDashStartLocation;
	FVector CachedDashDirection;
	
	ShootMode CurrentShootMode;
	float RapidShotCooldownTime;
	float BuckShotCooldownTime;
	
};
