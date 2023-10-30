// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaAnimInstance.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaDataAsset.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

DEFINE_LOG_CATEGORY(PCRLogEricaAnimInstance);

UPCREricaAnimInstance::UPCREricaAnimInstance()
	: ShouldMove(false), IsDashing(false), GroundSpeed(0.f),
	  LocalVelocityMoveDirectionAngle(0.f), CurrentLocalVelocityMoveDirection(ELocalVelocityDirection::Invalid),
	  LocalVelocityDashDirectionAngle(0.f), CurrentLocalVelocityDashDirection(ELocalVelocityDirection::Invalid),
	  CurrentIdleRotation(EIdleRotation::Invalid)
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

	LocalVelocityMoveDirectionAngle = UKismetAnimationLibrary::CalculateDirection(InputDirection, CachedEricaCharacter->GetActorRotation());
	
	SetLocalVelocityMoveDirectionAngle();
	SetIdleRotationDirection();

	IsDashing = CachedEricaCharacter->GetIsDashing();
	if (IsDashing)
	{
		LocalVelocityDashDirectionAngle = UKismetAnimationLibrary::CalculateDirection(CachedEricaCharacter->GetCachedDashDirection(), CachedEricaCharacter->GetActorRotation());
		SetLocalVelocityDashDirectionAngle();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Move: %d\tDash: %d "), CurrentLocalVelocityMoveDirection, CurrentLocalVelocityDashDirection);
	
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
		UE_LOG(PCRLogEricaAnimInstance, Log, TEXT("SectionNumber: %d"), InSectionNumber);
	}
}

void UPCREricaAnimInstance::SetLocalVelocityMoveDirectionAngle()
{
	const float ABSLocalVelocityMoveDirectionAngle = FMath::Abs(LocalVelocityMoveDirectionAngle);

	if (ABSLocalVelocityMoveDirectionAngle <= 22.5f)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::Forward;
	}
	else if (ABSLocalVelocityMoveDirectionAngle >= 157.5f)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::Backward;
	}
	else if (22.5f < LocalVelocityMoveDirectionAngle && LocalVelocityMoveDirectionAngle <= 67.5f)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::ForwardRight;
	}
	else if (-22.5f > LocalVelocityMoveDirectionAngle && LocalVelocityMoveDirectionAngle >= -67.5f)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::ForwardLeft;
	}
	else if (112.5f < LocalVelocityMoveDirectionAngle && LocalVelocityMoveDirectionAngle <= 157.5f)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::BackwardRight;
	}
	else if (-112.5f > LocalVelocityMoveDirectionAngle && LocalVelocityMoveDirectionAngle >= -157.5)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::BackwardLeft;
	}
	else if (LocalVelocityMoveDirectionAngle >= 0)
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::Right;
	}
	else
	{
		CurrentLocalVelocityMoveDirection = ELocalVelocityDirection::Left;
	}
}

void UPCREricaAnimInstance::SetLocalVelocityDashDirectionAngle()
{
	const float ABSLocalVelocityDashDirectionAngle = FMath::Abs(LocalVelocityDashDirectionAngle);

	if (ABSLocalVelocityDashDirectionAngle <= 22.5f)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::Forward;
	}
	else if (ABSLocalVelocityDashDirectionAngle >= 157.5f)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::Backward;
	}
	else if (22.5f < LocalVelocityDashDirectionAngle && LocalVelocityDashDirectionAngle <= 67.5f)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::ForwardRight;
	}
	else if (-22.5f > LocalVelocityDashDirectionAngle && LocalVelocityDashDirectionAngle >= -67.5f)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::ForwardLeft;
	}
	else if (112.5f < LocalVelocityDashDirectionAngle && LocalVelocityDashDirectionAngle <= 157.5f)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::BackwardRight;
	}
	else if (-112.5f > LocalVelocityDashDirectionAngle && LocalVelocityDashDirectionAngle >= -157.5)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::BackwardLeft;
	}
	else if (LocalVelocityDashDirectionAngle >= 0)
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::Right;
	}
	else
	{
		CurrentLocalVelocityDashDirection = ELocalVelocityDirection::Left;
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
