// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaAnimInstance.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaDataAsset.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

DEFINE_LOG_CATEGORY(PCRLogEricaAnimInstance);

UPCREricaAnimInstance::UPCREricaAnimInstance(): ShouldMove(false), IsDashing(false), GroundSpeed(0.f), LocalVelocityDirectionAngle(0.f), CurrentLocalVelocityDirection(ELocalVelocityDirection::Invalid)
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
	if (!CachedEricaCharacter)
	{
		NULL_POINTER_EXCEPTION(CachedEricaCharacter);
	}

	CachedCharacterMovement = CachedEricaCharacter ? CachedEricaCharacter->GetCharacterMovement() : nullptr;
	if (!CachedCharacterMovement)
	{
		NULL_POINTER_EXCEPTION(CachedCharacterMovement);
	}
}

void UPCREricaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedEricaCharacter)
	{
		return;
	}

	Velocity = CachedEricaCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	if (!CachedCharacterMovement)
	{
		return;
	}

	InputDirection = CachedCharacterMovement->GetCurrentAcceleration().GetSafeNormal();
	const bool IsMoveInput = InputDirection != FVector::ZeroVector;
	ShouldMove = IsMoveInput && GroundSpeed;

	EricaCharacterForwardDirection = CachedEricaCharacter->GetActorForwardVector();
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(InputDirection, CachedEricaCharacter->GetActorRotation());

	SetLocalVelocityDirectionAngle();

	IsDashing = CachedEricaCharacter->GetIsDashing();
}

void UPCREricaAnimInstance::Attack()
{
	Montage_Play(EricaDataAsset->AttackAnimationMontage);
	Montage_JumpToSection(FName(TEXT("TempCombo")), EricaDataAsset->AttackAnimationMontage);
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
