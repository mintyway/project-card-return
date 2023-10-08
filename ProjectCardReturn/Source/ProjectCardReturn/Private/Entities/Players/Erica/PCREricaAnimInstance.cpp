// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaAnimInstance.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

DEFINE_LOG_CATEGORY(PCRLogEricaAnimInstance);

UPCREricaAnimInstance::UPCREricaAnimInstance()
{
	GroundSpeed = 0.f;
	LocalVelocityDirectionAngle = 0.f;
	ShouldMove = false;
	IsDashing = false;
	CurrentLocalVelocityDirection = ELocalVelocityDirection::Invalid;
}

void UPCREricaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedOwningEricaCharacter = Cast<APCREricaCharacter>(GetOwningActor());
	RETURN_IF_INVALID(CachedOwningEricaCharacter);
	CachedOwningCharacterMovementComponent = CachedOwningEricaCharacter->GetCharacterMovement();
	RETURN_IF_INVALID(CachedOwningCharacterMovementComponent);
}

void UPCREricaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!CachedOwningEricaCharacter)
	{
		return;
	}
	
	Velocity = CachedOwningEricaCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	if (!CachedOwningCharacterMovementComponent)
	{
		return;
	}
	
	InputDirection = CachedOwningCharacterMovementComponent->GetCurrentAcceleration().GetSafeNormal();
	const bool IsMoveInput = InputDirection != FVector::ZeroVector;
	ShouldMove = IsMoveInput && GroundSpeed;

	EricaCharacterForwardDirection = CachedOwningEricaCharacter->GetActorForwardVector();
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(InputDirection, CachedOwningEricaCharacter->GetActorRotation());

	SetLocalVelocityDirectionAngle();
	
	IsDashing = CachedOwningEricaCharacter->GetIsDashing();
}

void UPCREricaAnimInstance::SetLocalVelocityDirectionAngle()
{
	const float ABSLocalVelocityDirectionAngle = FMath::Abs(LocalVelocityDirectionAngle);
	
	if (ABSLocalVelocityDirectionAngle <= 60)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Forward;
	}
	else if (ABSLocalVelocityDirectionAngle >= 120)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Backward;
	}
	else if (LocalVelocityDirectionAngle >= 0)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Right;
	}
	else
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Left;
	}
}
