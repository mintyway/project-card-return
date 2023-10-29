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
#include "NiagaraComponent.h"
#include "GameFramework/GameModeBase.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCharacter);

APCREricaCharacter::APCREricaCharacter()
	: bIsAlive(true), bCanDash(true), bIsDashing(false), bCanNarrowShot(true), bCanWideShot(true), bCanReturnCard(true), bCanAttack(true),
	  CurrentShotMode(ShootMode::NarrowShot),
	  MovementKeys{EKeys::W, EKeys::S, EKeys::D, EKeys::A},
	  ElapsedDashTime(0.f),
	  NarrowShotCount(3), NarrowShotElapsedCount(0), NarrowShotInterval(0.1f), WideShotCount(3),
	  CurrentCombo(0), MaxCombo(4), bIsAttackKeyPressed(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	Tags.AddUnique(TEXT("Erica"));

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
		MaxHP = ParameterDataAsset->EricaMaxHP;
		CurrentHP = MaxHP;

		WideShotAngle = ParameterDataAsset->EricaWideShotAngle;
		NarrowShotForwardDamage = ParameterDataAsset->EricaNarrowShotForwardDamage;
		NarrowShotBackwardDamage = ParameterDataAsset->EricaNarrowShotBackwardDamage;
		WideShotForwardDamage = ParameterDataAsset->EricaWideShotForwardDamage;
		WideShotBackwardDamage = ParameterDataAsset->EricaWideShotBackwardDamage;
		NarrowShotFiringRate = ParameterDataAsset->EricaNarrowShotFiringRate;
		WideShotFiringRate = ParameterDataAsset->EricaWideShotFiringRate;

		NarrowShotRange = ParameterDataAsset->EricaNarrowShotRange;
		WideShotRange = ParameterDataAsset->EricaWideShotRange;

		RecallCooldownTime = ParameterDataAsset->EricaRecallCooldownTime;

		DashCooldownTime = ParameterDataAsset->EricaDashCooldownTime;
		MaxDashTime = ParameterDataAsset->EricaMaxDashTime;
		DashDistance = ParameterDataAsset->EricaDashDistance;

		MaxCardCount = ParameterDataAsset->EricaCardCount;
		CurrentCardCount = MaxCardCount;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
		GetCapsuleComponent()->InitCapsuleSize(30.f, 75.f);
	}

	if (GetMesh() && EricaDataAsset)
	{
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
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
		CameraBoom->TargetOffset = FVector(ParameterDataAsset->CameraOffset);
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
		AimingPlane->SetRelativeScale3D(FVector(75.0, 75.0, 1.0));
		AimingPlane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AimingPlane->SetCollisionObjectType(ECC_GameTraceChannel4);
		AimingPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
		AimingPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Block);
		AimingPlane->SetHiddenInGame(true);
	}

	DashNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashNiagaraComponent"));
	if (DashNiagaraComponent && EricaDataAsset)
	{
		DashNiagaraComponent->SetupAttachment(GetCapsuleComponent());
		DashNiagaraComponent->SetAsset(EricaDataAsset->DashEffect);
	}
}

void APCREricaCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 데이터 에셋의 nullptr여부를 체크합니다.
	// 생성자에서 체크하지 않는 이유는 생성자는 게임을 시작한 상황이 아닌 일반 에디터 상황에서도 호출될 수 있고 이 상황이 nullptr일 수 있기 때문입니다.
	check(EricaDataAsset);

#if WITH_EDITOR
	SetActorLabel(TEXT("Erica"));
	SetFolderPath(TEXT("Erica"));
# endif
	
	check(DashNiagaraComponent);
	DashNiagaraComponent->Deactivate();

	// 카드 풀을 생성하는 코드입니다.
	CardProjectilePool = GetWorld()->SpawnActor<APCREricaCardProjectilePool>(FVector::ZeroVector, FRotator::ZeroRotator);
	check(CardProjectilePool);
	CardProjectilePool->InitProjectilePool(APCREricaCardProjectile::StaticClass());
	CardProjectilePool->OnCardRelease.BindUObject(this, &APCREricaCharacter::HandleRemoveInUseCard);

	// 애님 인스턴스를 캐싱해두는 코드입니다.
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	CachedEricaAnimInstance = Cast<UPCREricaAnimInstance>(AnimInstance);
	check(CachedEricaAnimInstance)

	CachedEricaAnimInstance->OnChainable.BindUObject(this, &APCREricaCharacter::HandleOnChainable);
	CachedEricaAnimInstance->OnChainEnd.BindUObject(this, &APCREricaCharacter::HandleOnChainEnd);
}

void APCREricaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(EricaDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &APCREricaCharacter::Move);
		EnhancedInputComponent->BindAction(EricaDataAsset->DashInputAction, ETriggerEvent::Started, this, &APCREricaCharacter::Dash);
		EnhancedInputComponent->BindAction(EricaDataAsset->ChangeInputAction, ETriggerEvent::Started, this, &APCREricaCharacter::Change);
		EnhancedInputComponent->BindAction(EricaDataAsset->ShootInputAction, ETriggerEvent::Triggered, this, &APCREricaCharacter::Attack);
		EnhancedInputComponent->BindAction(EricaDataAsset->ReturnInputAction, ETriggerEvent::Triggered, this, &APCREricaCharacter::RecallCard);
	}
}

void APCREricaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 사용중인 컨트롤러를 캐싱해두는 코드입니다.
	CachedEricaPlayerController = Cast<APCREricaPlayerController>(NewController);
	check(CachedEricaPlayerController);
}

void APCREricaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCREricaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 만약 플레이어가 죽었다면 Tick 함수 내 어떠한 동작도 수행하지 않도록 합니다.
	if (!bIsAlive)
	{
		return;
	}

	// 플레이어가 항상 마우스 방향을 바라보도록 합니다.
	const FRotator MouseDirectionRotator = FRotationMatrix::MakeFromX(CachedEricaPlayerController->GetMouseDirection()).Rotator();
	SetActorRotation(MouseDirectionRotator);

	if (bCanChainable && bIsAttackKeyPressed)
	{
		HandleCombo();
	}

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
void APCREricaCharacter::RecallCard()
{
	if (bCanReturnCard)
	{
		if (InUseCardProjectiles.IsEmpty())
		{
			return;
		}

		bool bIsCardReturnable = false;
		for (const auto& CardProjectile : InUseCardProjectiles)
		{
			if (CardProjectile->GetCurrentCardState() == ECardState::Stop)
			{
				CardProjectile->ReturnCard();
				bIsCardReturnable = true;
			}
		}

		if (bIsCardReturnable)
		{
			bCanReturnCard = false;

			FTimerHandle ReturnCardCooldownTimerHandle;
			FTimerDelegate ReturnCardCooldownTimerDelegate;
			ReturnCardCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ReturnCardCooldownTimerCallback);
			GetWorldTimerManager().SetTimer(ReturnCardCooldownTimerHandle, ReturnCardCooldownTimerDelegate, RecallCooldownTime, false);
		}
	}
}

float APCREricaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeHP(-ActualDamage);
	return ActualDamage;
}

void APCREricaCharacter::Attack()
{
	LastMouseClickedLocation = CachedEricaPlayerController->GetMouseDirection();

	if (bCanAttack)
	{
		CachedEricaAnimInstance->PlayAttackMontage();

		FOnMontageEnded AttackEndedDelegate;
		AttackEndedDelegate.BindUObject(this, &APCREricaCharacter::HandleOnAttackMontageEnded);
		CachedEricaAnimInstance->Montage_SetEndDelegate(AttackEndedDelegate, EricaDataAsset->AttackAnimationMontage);

		CurrentCombo = 1;
		bCanAttack = false;
	}
	else
	{
		bIsAttackKeyPressed = true;
	}
}

void APCREricaCharacter::HandleCombo()
{
	++CurrentCombo;
	if (CurrentCombo > MaxCombo)
	{
		CurrentCombo = 1;
	}

	CachedEricaAnimInstance->JumpToAttackMontageSection(CurrentCombo);
	bCanChainable = false;
}

void APCREricaCharacter::HandleOnChainable()
{
	bIsAttackKeyPressed = false;
	bCanChainable = true;
}

void APCREricaCharacter::HandleOnChainEnd()
{
	bCanChainable = false;
}

void APCREricaCharacter::HandleOnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bCanAttack = true;
	CurrentCombo = 0;
	bIsAttackKeyPressed = false;
	bCanChainable = false;
}

/**
 * 플레이어를 움직입니다.
 * @param InputActionValue 입력된 키 정보
 */
void APCREricaCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (!bIsAlive)
	{
		return;
	}

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
	switch (CurrentShotMode)
	{
		case ShootMode::NarrowShot:
		{
			NarrowShot();
			break;
		}
		case ShootMode::WideShot:
		{
			WideShot();
			break;
		}
		default:
		{
			break;
		}
	}
}

/**
 * 내로우샷 모드의 발사입니다.
 */
void APCREricaCharacter::NarrowShot()
{
	if (bCanNarrowShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (NarrowShotFiringRate > SMALL_NUMBER)
		{
			bCanNarrowShot = false;
			bCanWideShot = false;

			FTimerHandle ShotCooldownTimerHandle;
			FTimerDelegate ShotCooldownTimerDelegate;
			ShotCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ShotCooldownTimerCallback);

			const float CoolDownTime = 1.f / NarrowShotFiringRate;
			GetWorldTimerManager().SetTimer(ShotCooldownTimerHandle, ShotCooldownTimerDelegate, CoolDownTime, false);
		}

		// TODO: 공격 방향고민중
		// 첫발을 먼저 발사합니다.
		// FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		FVector MouseDirection = LastMouseClickedLocation;
		HandleNarrowShot(GetActorLocation(), MouseDirection);

		// 좌우 방향에서 2발이 순서대로 발사됩니다.
		FTimerDelegate NarrowShotDelayDelegate;
		TArray<FVector> ShotStartLocation;
		ShotStartLocation.Emplace(GetActorLocation() + GetActorRightVector() * 30);
		ShotStartLocation.Emplace(GetActorLocation() - GetActorRightVector() * 30);
		for (int32 i = 1; i < NarrowShotCount; ++i)
		{
			FTimerHandle NarrowShotDelayHandle;
			NarrowShotDelayDelegate.BindUObject(this, &APCREricaCharacter::HandleNarrowShot, ShotStartLocation[i - 1], MouseDirection);
			GetWorldTimerManager().SetTimer(NarrowShotDelayHandle, NarrowShotDelayDelegate, NarrowShotInterval * i, false);
		}
	}
}

// ReSharper disable CppPassValueParameterByConstReference
void APCREricaCharacter::HandleNarrowShot(FVector StartLocation, FVector MouseDirection)
{
	HandleShootCard(StartLocation, MouseDirection, NarrowShotForwardDamage, NarrowShotBackwardDamage, NarrowShotRange);
}

// ReSharper restore CppPassValueParameterByConstReference

/**
 * 와이드샷 모드의 발사입니다.
 */
void APCREricaCharacter::WideShot()
{
	if (bCanWideShot)
	{
		// 연사속도를 0으로 주면 쿨타임을 제거합니다.
		if (WideShotFiringRate > SMALL_NUMBER)
		{
			bCanNarrowShot = false;
			bCanWideShot = false;

			FTimerHandle ShotCooldownTimerHandle;
			FTimerDelegate ShotCooldownTimerDelegate;
			ShotCooldownTimerDelegate.BindUObject(this, &APCREricaCharacter::ShotCooldownTimerCallback);

			const float CoolDownTime = 1.f / WideShotFiringRate;
			GetWorldTimerManager().SetTimer(ShotCooldownTimerHandle, ShotCooldownTimerDelegate, CoolDownTime, false);
		}

		// TODO: 공격 방향고민중
		// const FVector MouseDirection = CachedEricaPlayerController->GetMouseDirection();
		const FVector MouseDirection = LastMouseClickedLocation;
		const float DegreeInterval = WideShotAngle / (WideShotCount - 1);

		for (int32 i = 0; i < WideShotCount; ++i)
		{
			const float CurrentRotation = -WideShotAngle / 2 + DegreeInterval * i;
			FQuat QuatRotation = FQuat::MakeFromEuler(FVector(0.0, 0.0, CurrentRotation));
			FVector CurrentDirection = QuatRotation.RotateVector(MouseDirection);
			HandleShootCard(GetActorLocation(), CurrentDirection, WideShotForwardDamage, WideShotBackwardDamage, WideShotRange);
		}
	}
}

void APCREricaCharacter::HandleShootCard(const FVector& StartLocation, const FVector& Direction, float ForwardDamage, float BackwardDamage, float Range)
{
	APCREricaCardProjectile* CardProjectile = Cast<APCREricaCardProjectile>(CardProjectilePool->Acquire());
	if (CardProjectile)
	{
		InUseCardProjectiles.Insert(CardProjectile, 0);
		CardProjectile->SetDamage(ForwardDamage, BackwardDamage);
		CardProjectile->SetRange(Range);
		CardProjectile->LaunchProjectile(this, StartLocation, Direction);
	}

	HandleChangeInUseCardsCount();
}

/**
 * 대시에 필요한 정보를 할당하고 대시 상태로 전환합니다.
 */
void APCREricaCharacter::Dash()
{
	if (!bIsAlive)
	{
		return;
	}

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

		const FRotator DashRotation = FRotationMatrix::MakeFromX(CachedDashDirection).Rotator();
		DashNiagaraComponent->SetWorldRotation(DashRotation);
		DashNiagaraComponent->Activate(true);
		SetActorRotation(DashRotation);
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
	if (!bIsAlive)
	{
		return;
	}

	switch (CurrentShotMode)
	{
		case ShootMode::NarrowShot:
		{
			CurrentShotMode = ShootMode::WideShot;
			break;
		}
		case ShootMode::WideShot:
		{
			CurrentShotMode = ShootMode::NarrowShot;
			break;
		}
		default:
		{
			CurrentShotMode = ShootMode::NarrowShot;
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
	bCanNarrowShot = true;
	bCanWideShot = true;
}

void APCREricaCharacter::DashCooldownTimerCallback()
{
	bCanDash = true;
}

void APCREricaCharacter::TotalDashTimeCallback()
{
	ElapsedDashTime = 0.f;
	bIsDashing = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
}

void APCREricaCharacter::ChangeHP(float Amount)
{
	CurrentHP += Amount;
	HandleChangeHP();
}

void APCREricaCharacter::HandleChangeHP()
{
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		HandleDead();
	}

	OnChangeHP.Broadcast(MaxHP, CurrentHP);
}

void APCREricaCharacter::HandleDead()
{
	UE_LOG(PCRLogEricaCharacter, Log, TEXT("%s 주인공 캐릭터가 죽었습니다."), *this->GetName());

	bIsAlive = false;
	DisableInput(CachedEricaPlayerController);

	OnDead.Broadcast();
}

void APCREricaCharacter::HandleRemoveInUseCard(APCREricaCardProjectile* EricaCard)
{
	InUseCardProjectiles.Remove(EricaCard);
	HandleChangeInUseCardsCount();
}

void APCREricaCharacter::HandleChangeInUseCardsCount()
{
	CurrentCardCount = MaxCardCount - InUseCardProjectiles.Num();
	OnChangeCardCount.Broadcast(MaxCardCount, CurrentCardCount);
}
