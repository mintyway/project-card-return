// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaAnimInstance.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaDataAsset.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

DEFINE_LOG_CATEGORY(PCRLogEricaAnimInstance);

UPCREricaAnimInstance::UPCREricaAnimInstance(): ShouldMove(false), IsDashing(false), GroundSpeed(0.f), LocalVelocityDirectionAngle(0.f), CurrentLocalVelocityDirection(ELocalVelocityDirection::Invalid), CurrentIdleRotation(EIdleRotation::Invalid)
{
	static ConstructorHelpers::FObjectFinder<UPCREricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.PCREricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		EricaDataAsset = DA_Erica.Object;
	}
}

void UPCREricaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedEricaCharacter = Cast<APCREricaCharacter>(GetOwningActor());
	CachedCharacterMovement = CachedEricaCharacter ? CachedEricaCharacter->GetCharacterMovement() : nullptr;
}

void UPCREricaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedEricaCharacter)
	{
		return;
	}

	check(CachedCharacterMovement);

	Velocity = CachedEricaCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	InputDirection = CachedCharacterMovement->GetCurrentAcceleration().GetSafeNormal();
	const bool IsMoveInput = InputDirection != FVector::ZeroVector;
	ShouldMove = IsMoveInput && GroundSpeed;

	EricaCharacterForwardDirection = CachedEricaCharacter->GetActorForwardVector();
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(InputDirection, CachedEricaCharacter->GetActorRotation());

	SetLocalVelocityDirectionAngle();
	SetIdleRotationDirection();

	IsDashing = CachedEricaCharacter->GetIsDashing();
	LastRotation = CachedEricaCharacter->GetActorRotation();
}

void UPCREricaAnimInstance::PlayAttackMontage()
{
	Montage_Play(EricaDataAsset->AttackAnimationMontage);
}

void UPCREricaAnimInstance::JumpToAttackMontageSection(int32 InSectionNumber)
{
	if (Montage_IsPlaying(EricaDataAsset->AttackAnimationMontage))
	{
		const FString SectionString = FString::Printf(TEXT("Combo%d"), InSectionNumber);
		const FName SectionName(SectionString);
		Montage_JumpToSection(SectionName, EricaDataAsset->AttackAnimationMontage);
		UE_LOG(LogTemp, Warning, TEXT("SectionNumber: %d"), InSectionNumber);
	}
}

void UPCREricaAnimInstance::SetLocalVelocityDirectionAngle()
{
	const float ABSLocalVelocityDirectionAngle = FMath::Abs(LocalVelocityDirectionAngle);

	if (ABSLocalVelocityDirectionAngle <= 22.5f)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Forward;
	}
	else if (ABSLocalVelocityDirectionAngle >= 157.5f)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::Backward;
	}
	else if (22.5f < LocalVelocityDirectionAngle && LocalVelocityDirectionAngle <= 67.5f)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::ForwardRight;
	}
	else if (-22.5f > LocalVelocityDirectionAngle && LocalVelocityDirectionAngle >= -67.5f)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::ForwardLeft;
	}
	else if (112.5f < LocalVelocityDirectionAngle && LocalVelocityDirectionAngle <= 157.5f)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::BackwardRight;
	}
	else if (-112.5f > LocalVelocityDirectionAngle && LocalVelocityDirectionAngle >= -157.5)
	{
		CurrentLocalVelocityDirection = ELocalVelocityDirection::BackwardLeft;
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

void UPCREricaAnimInstance::SetIdleRotationDirection()
{
	const FRotator DeltaRotation = CachedEricaCharacter->GetActorRotation() - LastRotation;
	if (DeltaRotation == FRotator::ZeroRotator)
	{
		CurrentIdleRotation = EIdleRotation::None;
	}
	else if (DeltaRotation.Yaw > 0.0)
	{
		CurrentIdleRotation = EIdleRotation::Right;
	}
	else
	{
		CurrentIdleRotation = EIdleRotation::Left;
	}
}

void UPCREricaAnimInstance::AnimNotify_Shoot()
{
	CachedEricaCharacter->ShootCard();
}

void UPCREricaAnimInstance::AnimNotify_Chainable()
{
	if (OnChainable.IsBound())
	{
		OnChainable.Execute();
	}
}

void UPCREricaAnimInstance::AnimNotify_ChainEnd()
{
	if (OnChainEnd.IsBound())
	{
		OnChainEnd.Execute();
	}
}
