// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCREricaAnimInstance.generated.h"

class UPCREricaDataAsset;
class UCharacterMovementComponent;
class APCREricaCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaAnimInstance, Log, All);

DECLARE_DELEGATE(FAttackEventSignature);
DECLARE_DYNAMIC_DELEGATE(FGameOverUISignature);

UENUM(BlueprintType)
enum class EIdleRotation : uint8
{
	None,
	Left,
	Right,
	Invalid
};

UENUM(BlueprintType)
enum class ELocalVelocityDirection : uint8
{
	Forward,
	ForwardRight,
	Right,
	BackwardRight,
	Backward,
	BackwardLeft,
	Left,
	ForwardLeft,
	Invalid
};


/**
 * 
 */
UCLASS

()
class PROJECTCARDRETURN_API UPCREricaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCREricaAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public: // 동작
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 InSectionNumber);

	void PlayRecallMontage();

	void PlayDeadMontage();

public: // 델리게이트
	FAttackEventSignature OnChainable;
	FAttackEventSignature OnChainEnd;
	FGameOverUISignature OnGameOverUI;

protected:
	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;

private:
	void SetLocalVelocityMoveDirectionAngle();
	void SetLocalVelocityDashDirectionAngle();
	void SetIdleRotationDirection();

	UFUNCTION()
	void AnimNotify_Shoot();

	UFUNCTION()
	void AnimNotify_Chainable();

	UFUNCTION()
	void AnimNotify_ChainEnd();

	UFUNCTION()
	void AnimNotify_OnGameOverUI();

private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CachedCharacterMovement;

private: // 로코모션
	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	uint32 IsAlive : 1;
	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	uint32 ShouldMove : 1;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	uint32 IsDashing : 1;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	FVector InputDirection;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	float LocalVelocityMoveDirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	ELocalVelocityDirection CurrentLocalVelocityMoveDirection;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	float LocalVelocityDashDirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	ELocalVelocityDirection CurrentLocalVelocityDashDirection;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	EIdleRotation CurrentIdleRotation;

	FRotator LastRotation;
};
