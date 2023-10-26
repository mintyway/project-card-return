// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
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
class UNiagaraComponent;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FAttackSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeHPSignature, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeCardCountSignature, int32, int32);
DECLARE_MULTICAST_DELEGATE(FDeadSignature);

/**
 * 캐릭터의 공격 모드를 설정합니다.
 */
UENUM()
enum class ShootMode : uint8
{
	NarrowShot,
	WideShot
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

public: // 동작
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ShootCard();
	void RecallCard();

public: // 델리게이트
	FAttackSignature OnCardShoot;
	FAttackSignature OnCardRecall;

	FChangeHPSignature OnChangeHP;
	FDeadSignature OnDead;

	FChangeCardCountSignature OnChangeCardCount;

public: // Getter, Setter
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

	// FORCEINLINE float GetSingleShotForwardDamage() const { return NarrowShotForwardDamage; }
	// FORCEINLINE float GetSingleShotBackwardDamage() const { return NarrowShotBackwardDamage; }
	// FORCEINLINE float GetMultiShotForwardDamage() const { return WideShotForwardDamage; }
	// FORCEINLINE float GetMultiShotBackwardDamage() const { return WideShotBackwardDamage; }

	FORCEINLINE int32 GetMaxCardCount() const { return MaxCardCount; }
	FORCEINLINE int32 GetCurrentCardCount() const { return CurrentCardCount; }

	bool GetIsDashing() const { return bIsDashing; }

private: // 내부 함수
	void Move(const FInputActionValue& InputActionValue);
	void Dash();
	void HandleDash(float DeltaTime);

	void HandleShootMode();
	void NarrowShot();
	void HandleNarrowShot(FVector StartLocation, FVector MouseDirection);
	void WideShot();
	void HandleShootCard(const FVector& StartLocation, const FVector& Direction, float ForwardDamage, float BackwardDamage, float Range);
	void Change();

	void ReturnCardCooldownTimerCallback();
	void ShotCooldownTimerCallback();
	void DashCooldownTimerCallback();
	void TotalDashTimeCallback();

	void ChangeHP(float Amount);
	void HandleChangeHP();
	void HandleDead();

	void HandleRemoveInUseCard(APCREricaCardProjectile* ApcrEricaCardProjectile);
	void HandleChangeInUseCardsCount();

private: // 데이터 에셋
	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Aim")
	TObjectPtr<UStaticMeshComponent> AimingPlane;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> DashNiagaraComponent;
	
private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaPlayerController> CachedEricaPlayerController;

	UPROPERTY()
	TObjectPtr<UPCREricaAnimInstance> CachedEricaAnimInstance;


private: // 스탯
	float MaxHP;
	float CurrentHP;
	
	float NarrowShotFiringRate;
	float WideShotFiringRate;
	
	float RecallCooldownTime;
	
	float NarrowShotForwardDamage;
	float NarrowShotBackwardDamage;
	float WideShotForwardDamage;
	float WideShotBackwardDamage;

	float NarrowShotRange;
	float WideShotRange;

private: // 상태
	uint32 bIsAlive : 1;

	uint32 bCanDash : 1;
	uint32 bIsDashing : 1;
	
	uint32 bCanNarrowShot : 1;
	uint32 bCanWideShot : 1;
	uint32 bCanReturnCard : 1;

	ShootMode CurrentShotMode;

private: // 대시
	TArray<FKey> MovementKeys;
	FVector LastInputMoveDirection;

	FVector CachedDashStartLocation;
	FVector CachedDashDirection;
	
	float DashCooldownTime;
	float MaxDashTime;
	
	float ElapsedDashTime;
	float DashDistance;
	
private: // 카드
	UPROPERTY()
	TObjectPtr<APCREricaCardProjectilePool> CardProjectilePool;

	UPROPERTY()
	TArray<TObjectPtr<APCREricaCardProjectile>> InUseCardProjectiles;

	int32 MaxCardCount;
	int32 CurrentCardCount;

	int32 NarrowShotCount;
	int32 NarrowShotElapsedCount;
	float NarrowShotInterval;
	
	int32 WideShotCount;
	float WideShotAngle;
};
