// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/CEricaCharacter.h"
#include "Entities/Players/Erica/CEricaCharacter.h"

#include "Entities/Players/Erica/CEricaDataAsset.h"
#include "Entities/Players/Erica/CEricaPlayerController.h"
#include "Entities/Projectiles/CEricaCardProjectile.h"
#include "Entities/Projectiles/CEricaCardProjectilePool.h"
#include "Game/CGameInstance.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

ACEricaCharacter::ACEricaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCEricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.CEricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		EricaDataAsset = DA_Erica.Object;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
		GetCapsuleComponent()->InitCapsuleSize(1.f, 77.f);
	}

	if (GetMesh() && EricaDataAsset)
	{
		GetMesh()->SetSkeletalMesh(EricaDataAsset->GetSkeletalMesh());
		GetMesh()->SetRelativeLocationAndRotation(FVector(8.0, 0.0, -77.0), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	if (GetCharacterMovement())
	{
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
		CameraBoom->TargetArmLength = 1000.f;
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
	CurrentShootMode = ShootMode::Rapid;
	bCanRapidShoot = true;
	RapidShootCoolTime = 0.0f;
}

void ACEricaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		RETURN_IF_INVALID(EricaDataAsset)
		EnhancedInputComponent->BindAction(EricaDataAsset->GetMoveInputAction(), ETriggerEvent::Triggered, this, &ACEricaCharacter::Move);
	}
}

void ACEricaCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 카드 풀을 생성하는 코드입니다.
	RETURN_IF_INVALID(IsValid(GetWorld()));
	CardPool = GetWorld()->SpawnActor<ACEricaCardProjectilePool>(FVector::ZeroVector, FRotator::ZeroRotator);
	RETURN_IF_INVALID(IsValid(CardPool));
	CardPool->InitProjectilePool(ACEricaCardProjectile::StaticClass(), this);
}

void ACEricaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 사용중인 컨트롤러를 캐싱해두는 코드입니다.
	CachedEricaPlayerController = Cast<ACEricaPlayerController>(NewController);
	RETURN_IF_INVALID(CachedEricaPlayerController);
}

void ACEricaCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ACEricaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 필드에 카드를 발사합니다.
 * CurrentShootMode의 값에따라 발사모드가 달라집니다.
 */
void ACEricaCharacter::ShootCard()
{
	switch (CurrentShootMode)
	{
		case ShootMode::Rapid:
		{
			RapidShoot();

			break;
		}
		case ShootMode::Buckshot:
		{
			BuckshotShoot();

			break;
		}
		default:
		{
			break;
		}
	}
}

/**
 * 필드에 발사된 카드를 회수합니다.
 */
void ACEricaCharacter::ReturnCard()
{
	while (!CardProjectileArray.IsEmpty() && !CardProjectileArray.Last()->IsShooting())
	{
		ACEricaCardProjectile* ReturningCard = CardProjectileArray.Pop();
		ReturningCard->ReturnCard();
	}
}

void ACEricaCharacter::Move(const FInputActionValue& InputActionValue)
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

void ACEricaCharacter::RapidShoot()
{
	if (bCanRapidShoot)
	{
		if (RapidShootCoolTime > SMALL_NUMBER)
		{
			bCanRapidShoot = false;
		
			FTimerHandle CoolTimeHandle;
			GetWorldTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				bCanRapidShoot = true;
			}), RapidShootCoolTime, false);
		}
		
		FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		FRotator MouseDirectionRotator = FRotationMatrix::MakeFromX(MouseDirection).Rotator();

		RETURN_IF_INVALID(CardPool);
		ACEricaCardProjectile* CardProjectile = Cast<ACEricaCardProjectile>(CardPool->GetProjectile(GetActorLocation()));
		RETURN_IF_INVALID(IsValid(CardProjectile));
		CardProjectileArray.Insert(CardProjectile, 0);
		UE_LOG(LogTemp, Warning, TEXT("%d"), CardProjectileArray.Num());
		CardProjectile->Shoot(MouseDirectionRotator.Vector());
	}
}

void ACEricaCharacter::BuckshotShoot()
{
	UE_LOG(LogTemp, Warning, TEXT("BuckshotShoot!"));
}
