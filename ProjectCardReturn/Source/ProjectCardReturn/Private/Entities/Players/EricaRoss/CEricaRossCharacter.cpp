// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/EricaRoss/CEricaRossCharacter.h"

#include "Entities/Players/EricaRoss/CEricaRossDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

ACEricaRossCharacter::ACEricaRossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCEricaRossDataAsset> DA_EricaRoss(TEXT("/Script/ProjectCardReturn.CEricaRossDataAsset'/Game/DataAssets/DA_EricaRoss.DA_EricaRoss'"));
	if (DA_EricaRoss.Succeeded())
	{
		DataAsset = DA_EricaRoss.Object;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
		GetCapsuleComponent()->InitCapsuleSize(1.f, 77.f);
	}

	if (GetMesh() && DataAsset)
	{
		GetMesh()->SetSkeletalMesh(DataAsset->GetMesh());
		GetMesh()->SetRelativeLocationAndRotation(FVector(8.0, 0.0, -77.0), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->BrakingFriction = 1.f;
		GetCharacterMovement()->MaxAcceleration = 999999.f;
		// 파라미터화 필요
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		GetCharacterMovement()->RotationRate = FRotator(0.0, 720.0, 0.0);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(GetCapsuleComponent());
		CameraBoom->SetRelativeRotation(FRotator(-45.0, 0.0, 0.0));
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritYaw = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->bDoCollisionTest = false;
		CameraBoom->bEnableCameraLag = true;
		// 파라미터화 필요
		CameraBoom->TargetArmLength = 800.f;
		// 파라미터화 필요
		CameraBoom->CameraLagSpeed = 10.f;
		// 파라미터화 필요
		CameraBoom->CameraLagMaxDistance = 100.f;
	}

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom);
	}

	bUseControllerRotationYaw = false;
	CurrentShootMode = ShootMode::Auto;
}

void ACEricaRossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		RETURN_IF_INVALID(DataAsset)
		EnhancedInputComponent->BindAction(DataAsset->GetMoveInputAction(), ETriggerEvent::Triggered, this, &ACEricaRossCharacter::Move);
	}
}

void ACEricaRossCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACEricaRossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEricaRossCharacter::Shoot()
{
	switch (CurrentShootMode)
	{
		case ShootMode::Auto:
		{
			
			
			break;
		}
		default:
		{
			break;
		}
	}
}

void ACEricaRossCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MoveScalar = InputActionValue.Get<FVector2D>();
	MoveScalar.Normalize();

	FRotator CameraRotation = CameraBoom->GetRelativeRotation();
	CameraRotation.Pitch = 0.0;

	FVector FrontDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	FVector SideDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(FrontDirection, static_cast<float>(MoveScalar.X));
	AddMovementInput(SideDirection, static_cast<float>(MoveScalar.Y));

	FVector MoveDirection = (FrontDirection * MoveScalar.X) + (SideDirection * MoveScalar.Y);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
}
