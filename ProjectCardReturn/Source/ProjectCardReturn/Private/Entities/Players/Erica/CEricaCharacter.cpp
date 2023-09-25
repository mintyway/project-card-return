// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/CEricaCharacter.h"
#include "Entities/Players/Erica/CEricaCharacter.h"

#include "Entities/Players/Erica/CEricaDataAsset.h"
#include "Entities/Players/Erica/CEricaPlayerController.h"
#include "Entities/Projectiles/EricaCard/CEricaCardProjectile.h"
#include "Entities/Projectiles/EricaCard/CEricaCardProjectilePool.h"
#include "Game/CGameInstance.h"
#include "Game/CParameterDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

ACEricaCharacter::ACEricaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	MovementKeys = {EKeys::W, EKeys::S, EKeys::D, EKeys::A};
	CurrentShootMode = ShootMode::Rapid;
	bCanRapidShot = true;
	bCanBuckShot = true;

	bCanDash = true;
	bIsDashing = false;

	ElapsedDashTime = 0.f;

	static ConstructorHelpers::FObjectFinder<UCEricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.CEricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		EricaDataAsset = DA_Erica.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.CParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	if (IsValid(ParameterDataAsset))
	{
		AttackPower = ParameterDataAsset->GetEricaAttackPower();
		RapidShotCoolTime = ParameterDataAsset->GetEricaRapidShotCoolTime();
		BuckShotCoolTime = ParameterDataAsset->GetEricaBuckShotCoolTime();
		DashCoolTime = ParameterDataAsset->GetEricaDashCoolTime();
		TotalDashTime = ParameterDataAsset->GetEricaTotalDashTime();
		DashDistance = ParameterDataAsset->GetEricaDashDistance();
	}

	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
		GetCapsuleComponent()->InitCapsuleSize(1.f, 77.f);
	}

	if (IsValid(GetMesh()) && IsValid(EricaDataAsset))
	{
		GetMesh()->SetSkeletalMesh(EricaDataAsset->GetSkeletalMesh());
		GetMesh()->SetRelativeLocationAndRotation(FVector(8.0, 0.0, -77.0), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	if (IsValid(GetCharacterMovement()) && IsValid(ParameterDataAsset))
	{
		GetCharacterMovement()->BrakingFriction = 1.f;
		GetCharacterMovement()->MaxAcceleration = 999999.f;
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->GetEricaMoveSpeed();
		GetCharacterMovement()->RotationRate = FRotator(0.0, 720.0, 0.0);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (IsValid(CameraBoom) && IsValid(ParameterDataAsset))
	{
		CameraBoom->SetupAttachment(GetCapsuleComponent());
		CameraBoom->SetRelativeRotation(FRotator(-45.0, 0.0, 0.0));
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritYaw = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->bDoCollisionTest = false;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->TargetArmLength = ParameterDataAsset->GetCameraDistance();
		CameraBoom->CameraLagSpeed = ParameterDataAsset->GetCameraLagSpeed();
		CameraBoom->CameraLagMaxDistance = ParameterDataAsset->GetCameraLagMaxDistance();
	}

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (IsValid(FollowCamera))
	{
		FollowCamera->SetupAttachment(CameraBoom);
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

void ACEricaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		RETURN_IF_INVALID(EricaDataAsset)
		EnhancedInputComponent->BindAction(EricaDataAsset->GetMoveInputAction(), ETriggerEvent::Triggered, this, &ACEricaCharacter::Move);
		EnhancedInputComponent->BindAction(EricaDataAsset->GetDashInputAction(), ETriggerEvent::Started, this, &ACEricaCharacter::Dash);
		EnhancedInputComponent->BindAction(EricaDataAsset->GetChangeInputAction(), ETriggerEvent::Started, this, &ACEricaCharacter::Change);
	}
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

	if (bIsDashing)
	{
		HandleDash(DeltaTime);
	}
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
			RapidShot();
			break;
		}
		case ShootMode::Buckshot:
		{
			BuckShot();
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

/**
 * 플레이어를 움직입니다.
 * @param InputActionValue 입력된 키 정보
 */
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
	LastInputMoveDirection = (FrontDirection * MoveScalar.X) + (SideDirection * MoveScalar.Y);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(LastInputMoveDirection).Rotator());
}

/**
 * 연발 모드의 발사입니다.
 */
void ACEricaCharacter::RapidShot()
{
	if (bCanRapidShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (RapidShotCoolTime > SMALL_NUMBER)
		{
			bCanRapidShot = false;
			bCanBuckShot = false;

			FTimerHandle CoolTimeHandle;
			GetWorldTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				bCanRapidShot = true;
				bCanBuckShot = true;
			}), RapidShotCoolTime, false);
		}

		RETURN_IF_INVALID(CachedEricaPlayerController);
		FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		FRotator MouseDirectionRotator = FRotationMatrix::MakeFromX(MouseDirection).Rotator();

		RETURN_IF_INVALID(IsValid(CardPool));
		ACEricaCardProjectile* CardProjectile = Cast<ACEricaCardProjectile>(CardPool->GetProjectile(GetActorLocation()));
		RETURN_IF_INVALID(IsValid(CardProjectile));
		CardProjectileArray.Insert(CardProjectile, 0);
		CardProjectile->Shoot(MouseDirectionRotator.Vector());
	}
}

/**
 * 벅샷 모드의 발사입니다.
 */
void ACEricaCharacter::BuckShot()
{
	if (bCanBuckShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (BuckShotCoolTime > SMALL_NUMBER)
		{
			bCanRapidShot = false;
			bCanBuckShot = false;
			FTimerHandle CoolTimeHandle;
			GetWorldTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				bCanRapidShot = true;
				bCanBuckShot = true;
			}), BuckShotCoolTime, false);
		}

		FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		int32 ProjectileCount = 5;
		float TotalDegrees = 90.f;
		float DegreeInterval = TotalDegrees / (ProjectileCount - 1);

		for (int32 i = 0; i < ProjectileCount; ++i)
		{
			float CurrentRotation = -TotalDegrees / 2 + DegreeInterval * i;

			FQuat QuatRotation = FQuat::MakeFromEuler(FVector(0.0, 0.0, CurrentRotation));
			FVector CurrentDirection = QuatRotation.RotateVector(MouseDirection);

			RETURN_IF_INVALID(IsValid(CardPool));
			ACEricaCardProjectile* CardProjectile = Cast<ACEricaCardProjectile>(CardPool->GetProjectile(GetActorLocation()));
			if (IsValid(CardProjectile))
			{
				CardProjectileArray.Insert(CardProjectile, 0);
				CardProjectile->Shoot(CurrentDirection);
			}
		}
	}
}

/**
 * 대시에 필요한 정보를 할당하고 대시 상태로 전환합니다.
 */
void ACEricaCharacter::Dash()
{
	SIMPLE_LOG;

	if (bCanDash)
	{
		bCanDash = false;
		bIsDashing = true;

		FTimerHandle DashCoolTimeHandle;
		GetWorldTimerManager().SetTimer(DashCoolTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
		{
			bCanDash = true;
		}), DashCoolTime, false);

		FTimerHandle TotalDashTimeHandle;
		GetWorldTimerManager().SetTimer(TotalDashTimeHandle, FTimerDelegate::CreateLambda([this]() -> void
		{
			ElapsedDashTime = 0.f;
			bIsDashing = false;
		}), TotalDashTime, false);

		CachedDashStartLocation = GetActorLocation();

		RETURN_IF_INVALID(IsValid(CachedEricaPlayerController));
		bool bAnyMovementKeyDown = false;
		for (const auto& MovementKey : MovementKeys)
		{
			if (CachedEricaPlayerController->IsInputKeyDown(MovementKey))
			{
				bAnyMovementKeyDown = true;
				break;
			}
		}

		if (bAnyMovementKeyDown)
		{
			CachedDashDirection = LastInputMoveDirection;
		}
		else
		{
			CachedDashDirection = CachedEricaPlayerController->GetMouseDirection();
		}

		SetActorRotation(FRotationMatrix::MakeFromX(CachedDashDirection).Rotator());
	}
}

void ACEricaCharacter::HandleDash(float DeltaTime)
{
	ElapsedDashTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedDashTime / TotalDashTime, 0.f, 1.0f);
	FVector NewLocation = FMath::Lerp(CachedDashStartLocation, CachedDashStartLocation + CachedDashDirection * DashDistance, Alpha);
	if (!SetActorLocation(NewLocation, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("Blocking"));
		bIsDashing = false;
	}
}

void ACEricaCharacter::Change()
{
	SIMPLE_LOG;

	switch (CurrentShootMode)
	{
		case ShootMode::Rapid:
		{
			CurrentShootMode = ShootMode::Buckshot;
			break;
		}
		case ShootMode::Buckshot:
		{
			CurrentShootMode = ShootMode::Rapid;
			break;
		}
		default:
		{
			CurrentShootMode = ShootMode::Rapid;
			break;
		}
	}
}
