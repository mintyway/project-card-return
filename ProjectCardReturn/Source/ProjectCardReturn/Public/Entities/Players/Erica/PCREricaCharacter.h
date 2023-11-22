// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "InputActionValue.h"
#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "GameFramework/Character.h"
#include "PCREricaCharacter.generated.h"

class APCRListenerActor;
class UPCRSoundPrimaryDataAsset;
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
enum class EShootMode : uint8
{
	NarrowShot,
	WideShot
};

DECLARE_DELEGATE_OneParam(FShootModeSignature, EShootMode);

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

	void Revival();
	
	void ShootCard();
	void RecallCard();
	void Heal(float Amount);
	void IncreaseMaxHP(float Amount);
	void IncreaseMaxCardCount(int32 Amount);
	void IncreaseDamage(float Amount);
	void IncreaseShootRange(float Amount);

	float CardAverageDamage();
	float CardAverageRange();

public: // 델리게이트
	FAttackSignature OnCardShoot;
	FAttackSignature OnCardRecall;

	FChangeHPSignature OnChangeHP;
	FDeadSignature OnDead;

	FChangeCardCountSignature OnChangeCardCount;
	FShootModeSignature OnChangeShootMode;

public: // Getter, Setter
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

	FORCEINLINE int32 GetMaxCardCount() const { return MaxCardCount; }
	FORCEINLINE int32 GetCurrentCardCount() const { return CurrentCardCount; }

	FORCEINLINE bool GetIsDashing() const { return bIsDashing; }

	FORCEINLINE const FVector& GetCachedDashDirection() const { return CachedDashDirection; }

private: // 내부 함수
	void Attack();
	void HandleCombo();
	void HandleOnChainable();
	void HandleOnChainEnd();
	void HandleOnAttackMontageEnded(UAnimMontage* AnimMontage, bool bArg);

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
	void DashInvincibleTimerCallback();

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

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Aim")
	TObjectPtr<UBoxComponent> AimingBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> DashNiagaraComponent;

private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaPlayerController> CachedEricaPlayerController;

	UPROPERTY()
	TObjectPtr<UPCREricaAnimInstance> CachedEricaAnimInstance;

	UPROPERTY()
	TObjectPtr<APCRListenerActor> CachedListenerActor;

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
	uint32 bIsAlive:1;

	uint32 bCanDash:1;
	uint32 bIsDashing:1;
	uint32 bIsDashInvincible:1;

	uint32 bCanNarrowShot:1;
	uint32 bCanWideShot:1;
	uint32 bCanReturnCard:1;

	uint32 bCanAttack:1;

	EShootMode CurrentShotMode;

private: // 대시
	TArray<FKey> MovementKeys;
	FVector LastInputMoveDirection;

	FVector CachedDashStartLocation;
	FVector CachedDashDirection;

	float DashCooldownTime;
	float MaxDashTime;
	float DashInvincibleTime;

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

private: // 공격
	FVector LastMouseClickedLocation;
	int32 CurrentCombo;
	int32 MaxCombo;
	uint32 bIsAttackKeyPressed:1;
	uint32 bCanChainable:1;
};
