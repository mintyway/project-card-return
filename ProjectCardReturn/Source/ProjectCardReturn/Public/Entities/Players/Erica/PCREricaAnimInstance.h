// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCREricaAnimInstance.generated.h"

class UPCREricaDataAsset;
class UCharacterMovementComponent;
class APCREricaCharacter;

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

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaAnimInstance, Log, All);

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

public:
	void Attack();

protected:
	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;
	
private:
	void SetLocalVelocityDirectionAngle();
	void SetIdleRotationDirection();

private:
	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	uint32 ShouldMove:1;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	uint32 IsDashing:1;
	
	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	FVector InputDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	FVector EricaCharacterForwardDirection;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	float LocalVelocityDirectionAngle;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	ELocalVelocityDirection CurrentLocalVelocityDirection;
	
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionData", meta = (AllowPrivateAccess = true))
	EIdleRotation CurrentIdleRotation;
	
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CachedCharacterMovement;

	
	FRotator LastRotation;
};
