// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

#include "Entities/Players/Erica/PCREricaDataAsset.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectilePool.h"
#include "Game/PCRGameInstance.h"
#include "Game/PCRParameterDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Entities/Players/Erica/PCREricaAnimInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCharacter);

APCREricaCharacter::APCREricaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	MovementKeys = {EKeys::W, EKeys::S, EKeys::D, EKeys::A};
	CurrentShootMode = ShootMode::Rapid;
	bCanRapidShot = true;
	bCanBuckShot = true;

	bCanDash = true;
	bIsDashing = false;
	bCanReturnCard = true;

	ElapsedDashTime = 0.f;
	// TODO: 파라미터화 필요
	ReturnCardCoolTime = 1.f;

	static ConstructorHelpers::FObjectFinder<UPCREricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.PCREricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		EricaDataAsset = DA_Erica.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	if (ParameterDataAsset)
	{
		AttackPower = ParameterDataAsset->EricaAttackPower;
		RapidShotCooldownTime = ParameterDataAsset->EricaRapidShotCooldownTime;
		BuckShotCooldownTime = ParameterDataAsset->EricaBuckShotCooldownTime;
		DashCooldownTime = ParameterDataAsset->EricaDashCooldownTime;
		MaxDashTime = ParameterDataAsset->EricaMaxDashTime;
		DashDistance = ParameterDataAsset->EricaDashDistance;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
		GetCapsuleComponent()->InitCapsuleSize(30.f, 75.f);
	}

	if (GetMesh() && EricaDataAsset)
	{
		GetMesh()->SetSkeletalMesh(EricaDataAsset->SkeletalMesh);
		GetMesh()->SetRelativeLocationAndRotation(FVector(8.0, 0.0, -77.0), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (UClass* EricaAnimationBlueprint = EricaDataAsset->AnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(EricaAnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && ParameterDataAsset)
	{
		GetCharacterMovement()->BrakingFriction = 1.f;
		GetCharacterMovement()->MaxAcceleration = 999999.f;
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->EricaMoveSpeed;
		GetCharacterMovement()->RotationRate = FRotator(0.0, 720.0, 0.0);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom && ParameterDataAsset)
	{
		CameraBoom->SetupAttachment(GetCapsuleComponent());
		CameraBoom->SetRelativeRotation(FRotator(-ParameterDataAsset->CameraPitch, 0.0, 0.0));
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritYaw = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->bDoCollisionTest = false;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->TargetArmLength = ParameterDataAsset->CameraDistance;
		CameraBoom->TargetOffset = FVector(ParameterDataAsset->CameraOffset, 0.0, 0.0);
		CameraBoom->CameraLagSpeed = ParameterDataAsset->CameraLagSpeed;
		CameraBoom->CameraLagMaxDistance = ParameterDataAsset->CameraLagMaxDistance;
	}

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera && ParameterDataAsset)
	{
		FollowCamera->SetupAttachment(CameraBoom);
		FollowCamera->SetFieldOfView(ParameterDataAsset->CameraFOV);
	}

	AimingPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimingPlane"));
	if (AimingPlane && EricaDataAsset)
	{
		AimingPlane->SetupAttachment(GetCapsuleComponent());
		AimingPlane->SetStaticMesh(EricaDataAsset->AimingPlane);
		AimingPlane->SetRelativeScale3D(FVector(50.0, 50.0, 1.0));
		AimingPlane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AimingPlane->SetCollisionObjectType(ECC_GameTraceChannel4);
		AimingPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
		AimingPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Block);
		AimingPlane->SetHiddenInGame(true);
	}
}

void APCREricaCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 카드 풀을 생성하는 코드입니다.
	RETURN_IF_INVALID(IsValid(GetWorld()));
	CardPool = GetWorld()->SpawnActor<APCREricaCardProjectilePool>(FVector::ZeroVector, FRotator::ZeroRotator);
	RETURN_IF_INVALID(IsValid(CardPool));
	CardPool->InitProjectilePool(APCREricaCardProjectile::StaticClass());

	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance)
	{
		NULL_POINTER_EXCEPTION(AnimInstance);
	}

	CachedEricaAnimInstance = Cast<UPCREricaAnimInstance>(AnimInstance);
	if (!CachedEricaAnimInstance)
	{
		NULL_POINTER_EXCEPTION(CachedEricaAnimInstance);
	}
}

void APCREricaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		RETURN_IF_INVALID(EricaDataAsset)
		EnhancedInputComponent->BindAction(EricaDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &APCREricaCharacter::Move);
		EnhancedInputComponent->BindAction(EricaDataAsset->DashInputAction, ETriggerEvent::Started, this, &APCREricaCharacter::Dash);
		EnhancedInputComponent->BindAction(EricaDataAsset->ChangeInputAction, ETriggerEvent::Started, this, &APCREricaCharacter::Change);
	}
}

void APCREricaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 사용중인 컨트롤러를 캐싱해두는 코드입니다.
	CachedEricaPlayerController = Cast<APCREricaPlayerController>(NewController);
	RETURN_IF_INVALID(CachedEricaPlayerController);
}

void APCREricaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCREricaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDashing)
	{
		HandleDash(DeltaTime);
	}
}

/**
 * 카드를 발사합니다.
 */
void APCREricaCharacter::ShootCard()
{
	HandleShootMode();
}

/**
 * 필드에 발사된 카드를 회수합니다.
 */
void APCREricaCharacter::ReturnCard()
{
	if (bCanReturnCard)
	{
		bCanReturnCard = false;

		FTimerHandle ReturnCardCooldownTimerHandle;
		FTimerDelegate ReturnCardCooldownTimerDelegate;
		ReturnCardCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ReturnCardCooldownTimerCallback);
		GetWorldTimerManager().SetTimer(ReturnCardCooldownTimerHandle, ReturnCardCooldownTimerDelegate, ReturnCardCoolTime, false);

		if (CardProjectiles.IsEmpty())
		{
			return;
		}

		TArray<APCREricaCardProjectile*> CardsToRemove;

		for (const auto& CardProjectile : CardProjectiles)
		{
			if (CardProjectile->GetCurrentCardState() == ECardState::Stop)
			{
				CardsToRemove.Add(CardProjectile);
			}
		}

		for (const auto& CardToRemove : CardsToRemove)
		{
			int32 Index;
			if (CardProjectiles.Find(CardToRemove, Index))
			{
				APCREricaCardProjectile* ReturningCard = CardProjectiles[Index];
				CardProjectiles.RemoveAt(Index);
				ReturningCard->ReturnCard();
			}
		}
	}
}

/**
 * 플레이어를 움직입니다.
 * @param InputActionValue 입력된 키 정보
 */
void APCREricaCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MoveScalar = InputActionValue.Get<FVector2D>();
	MoveScalar.Normalize();

	FRotator CameraRotation = CameraBoom->GetRelativeRotation();
	CameraRotation.Pitch = 0.0;

	const FVector FrontDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	const FVector SideDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(FrontDirection, static_cast<float>(MoveScalar.X));
	AddMovementInput(SideDirection, static_cast<float>(MoveScalar.Y));
	LastInputMoveDirection = (FrontDirection * MoveScalar.X) + (SideDirection * MoveScalar.Y);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(LastInputMoveDirection).Rotator());
}

/**
 * 현재 발사모드를 확인하고 이에 맞는 발사로직을 호출해줍니다.
 */
void APCREricaCharacter::HandleShootMode()
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
 * 연발 모드의 발사입니다.
 */
void APCREricaCharacter::RapidShot()
{
	if (bCanRapidShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (RapidShotCooldownTime > SMALL_NUMBER)
		{
			bCanRapidShot = false;
			bCanBuckShot = false;

			FTimerHandle ShotCooldownTimerHandle;
			FTimerDelegate ShotCooldownTimerDelegate;
			ShotCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ShotCooldownTimerCallback);
			GetWorldTimerManager().SetTimer(ShotCooldownTimerHandle, ShotCooldownTimerDelegate, RapidShotCooldownTime, false);
		}

		RETURN_IF_INVALID(CachedEricaPlayerController);
		const FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		HandleShootCard(MouseDirection, ParameterDataAsset->EricaCardRapidShotRange);
	}
}

/**
 * 벅샷 모드의 발사입니다.
 */
void APCREricaCharacter::BuckShot()
{
	if (bCanBuckShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (BuckShotCooldownTime > SMALL_NUMBER)
		{
			bCanRapidShot = false;
			bCanBuckShot = false;
			FTimerHandle ShotCooldownTimerHandle;
			FTimerDelegate ShotCooldownTimerDelegate;
			ShotCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ShotCooldownTimerCallback);
			GetWorldTimerManager().SetTimer(ShotCooldownTimerHandle, ShotCooldownTimerDelegate, BuckShotCooldownTime, false);
		}

		const FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		const int32 ProjectileCount = 5;
		const float TotalDegrees = 60.f;
		const float DegreeInterval = TotalDegrees / (ProjectileCount - 1);

		for (int32 i = 0; i < ProjectileCount; ++i)
		{
			const float CurrentRotation = -TotalDegrees / 2 + DegreeInterval * i;
			FQuat QuatRotation = FQuat::MakeFromEuler(FVector(0.0, 0.0, CurrentRotation));
			FVector CurrentDirection = QuatRotation.RotateVector(MouseDirection);
			HandleShootCard(CurrentDirection, ParameterDataAsset->EricaCardBuckShotRange);
		}
	}
}

void APCREricaCharacter::HandleShootCard(const FVector& Direction, float Range)
{
	RETURN_IF_INVALID(CardPool);
	APCREricaCardProjectile* CardProjectile = Cast<APCREricaCardProjectile>(CardPool->Acquire());
	if (CardProjectile)
	{
		CardProjectiles.Insert(CardProjectile, 0);
		CardProjectile->SetRange(Range);
		CardProjectile->LaunchProjectile(this, GetActorLocation(), Direction);

		if (CachedEricaAnimInstance)
		{
			CachedEricaAnimInstance->Attack();
		}
	}

	UE_LOG(PCRLogEricaCharacter, Log, TEXT("필드에 발사된 카드 개수: %d"), CardProjectiles.Num());
}

/**
 * 대시에 필요한 정보를 할당하고 대시 상태로 전환합니다.
 */
void APCREricaCharacter::Dash()
{
	if (bCanDash)
	{
		bCanDash = false;
		bIsDashing = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

		FTimerHandle DashCooldownTimerHandle;
		FTimerDelegate DashCooldownTimerDelegate;
		DashCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::DashCooldownTimerCallback);
		GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, DashCooldownTimerDelegate, DashCooldownTime, false);

		// 최대 대시 시간을 초과했을 시 상태를 초기화 해줍니다.
		FTimerHandle MaxDashTimerHandle;
		FTimerDelegate MaxDashTimerDelegate;
		MaxDashTimerDelegate.BindUObject(this, &APCREricaCharacter::TotalDashTimeCallback);
		GetWorldTimerManager().SetTimer(MaxDashTimerHandle, MaxDashTimerDelegate, MaxDashTime, false);

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

void APCREricaCharacter::HandleDash(float DeltaTime)
{
	ElapsedDashTime += DeltaTime;
	const float Alpha = FMath::Clamp(ElapsedDashTime / MaxDashTime, 0.f, 1.0f);
	const FVector NewLocation = FMath::Lerp(CachedDashStartLocation, CachedDashStartLocation + CachedDashDirection * DashDistance, Alpha);
	if (!SetActorLocation(NewLocation, true))
	{
		UE_LOG(PCRLogEricaCharacter, Log, TEXT("대시 중 벽에 막혔습니다."));
		bIsDashing = false;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}
}

void APCREricaCharacter::Change()
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

void APCREricaCharacter::ReturnCardCooldownTimerCallback()
{
	bCanReturnCard = true;
}

void APCREricaCharacter::ShotCooldownTimerCallback()
{
	bCanRapidShot = true;
	bCanBuckShot = true;
}

void APCREricaCharacter::DashCooldownTimerCallback()
{
	bCanDash = true;
}

void APCREricaCharacter::TotalDashTimeCallback()
{
	ElapsedDashTime = 0.f;
	bIsDashing = false;
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}
	else
	{
		NULL_POINTER_EXCEPTION(GetCapsuleComponent());
	}
}
