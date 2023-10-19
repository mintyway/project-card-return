// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "NiagaraComponent.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/Base/PCRProjectileDataAsset.h"
#include "Game/PCRParameterDataAsset.h"
#include "Interfaces/PCREricaCardInteractable.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCardProjectile);

APCREricaCardProjectile::APCREricaCardProjectile()
{
	CurrentCardState = ECardState::Invalid;

	if (GetParameterDataAsset())
	{
		ProjectileSpeed = GetParameterDataAsset()->EricaCardSpeed;
		CardReturnSpeed = GetParameterDataAsset()->EricaCardReturnSpeed;
		CardRange = GetParameterDataAsset()->EricaCardRapidShotRange;
		CardReleaseRange = GetParameterDataAsset()->EricaCardReleaseRange;
	}

	if (GetBoxComponent())
	{
		GetBoxComponent()->InitBoxExtent(FVector(30.8, 21.7, 1.0));
	}

	if (GetStaticMeshComponent() && GetProjectileDataAsset())
	{
		GetStaticMeshComponent()->SetStaticMesh(GetProjectileDataAsset()->EricaCardMesh);
		GetStaticMeshComponent()->SetRelativeScale3D(FVector(7.0, 7.0, 1.0));
	}

	if (GetProjectileMovementComponent())
	{
		GetProjectileMovementComponent()->MaxSpeed = ProjectileSpeed;
	}

	CardRibbonFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardRibbonFXComponent"));
	if (CardRibbonFXComponent)
	{
		CardRibbonFXComponent->SetupAttachment(GetBoxComponent());
		if (GetProjectileDataAsset()->CardRibbon)
		{
			CardRibbonFXComponent->SetAsset(GetProjectileDataAsset()->CardRibbon);
		}
	}

	CardFloatingFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardFloatingFXComponent"));
	if (CardFloatingFXComponent)
	{
		CardFloatingFXComponent->SetupAttachment(GetBoxComponent());
		CardFloatingFXComponent->SetAsset(GetProjectileDataAsset()->CardFloating);
	}
}

void APCREricaCardProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorBeginOverlap.AddDynamic(this, &APCREricaCardProjectile::HandleBeginOverlap);
	OnActorHit.AddDynamic(this, &APCREricaCardProjectile::HandleBlocking);

	CardRibbonFXComponent->Deactivate();
	DisableCardFloatingFX();
}

void APCREricaCardProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (CurrentCardState)
	{
		case ECardState::Flying:
		{
			CheckCardRangeAndStop(DeltaSeconds);
			break;
		}
		case ECardState::Stop:
		{
			return;
		}
		case ECardState::Returning:
		{
			HandleCardReturn(DeltaSeconds);
			break;
		}
		case ECardState::Invalid:
		{
			return;
		}
	}
}

void APCREricaCardProjectile::LaunchProjectile(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction)
{
	Super::LaunchProjectile(NewOwner, StartLocation, Direction);
	CardRibbonFXComponent->Activate();

	CurrentCardState = ECardState::Flying;
}

void APCREricaCardProjectile::ReleaseToProjectilePool()
{
	Super::ReleaseToProjectilePool();

	CardRibbonFXComponent->Deactivate();
	DisableCardFloatingFX();
	OnReturnCardBegin.Clear();
}

/**
 * 카드를 플레이어에게로 복귀하는 상태로 바꿉니다.
 */
void APCREricaCardProjectile::ReturnCard()
{
	UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 복귀를 시작합니다."), *GetName());
	CurrentCardState = ECardState::Returning;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EnableProjectile();
	CardRibbonFXComponent->Activate();
	DisableCardFloatingFX();
	GetStaticMeshComponent()->SetVisibility(true);

	OnReturnCardBegin.Broadcast(this);
}

void APCREricaCardProjectile::EnableCollisionDetection()
{
	if (GetBoxComponent())
	{
		GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetBoxComponent()->SetCollisionObjectType(ECC_GameTraceChannel3);
		GetBoxComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetBoxComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		GetBoxComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Block);
		GetBoxComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Block);
	}
}

/**
 * 카드를 멈추고 틱, 충돌을 비활성화합니다. 이 함수를 호출하면 카드는 복귀 가능 상태가 됩니다.
 */
void APCREricaCardProjectile::PauseCard()
{
	CurrentCardState = ECardState::Stop;
	DisableProjectile();
}

/**
 * 
 * @param OverlappedActor 카드 액터
 * @param OtherActor 카드에 맞은 액터
 */
void APCREricaCardProjectile::HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!OtherCharacter)
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s가 %s와 충돌했지만 %s이 물체이기 때문에 데미지를 주지 못했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName(), *OtherActor->GetName());

		return;
	}

	const FVector CurrentDirection = OverlappedActor->GetActorForwardVector();
	const FVector CurrentOtherActorDirection = OtherCharacter->GetActorForwardVector();

	const float DotResult = FVector::DotProduct(CurrentDirection, CurrentOtherActorDirection);
	if (DotResult <= 0)
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 %s와 정면으로 충돌했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName());
		// TODO: 현재는 해당 액터가 밀리면서 여러번 오버랩될 수 있는 상황입니다. 나중에 충돌 당하는 액터에게 오버랩 쿨타임을 구현해 이를 막아야합니다.
		OtherCharacter->LaunchCharacter(OverlappedActor->GetActorForwardVector() * 500.f, false, false);

		const APCREricaCharacter* EricaCharacter = Cast<APCREricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherCharacter->TakeDamage(EricaCharacter->GetAttackPower(), DamageEvent, EricaCharacter->GetController(), this);
	}
	else
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 %s와 후면으로 충돌했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName());
		APCREricaCharacter* EricaCharacter = Cast<APCREricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(EricaCharacter->GetAttackPower(), DamageEvent, EricaCharacter->GetController(), EricaCharacter);
	}
}

void APCREricaCardProjectile::HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 블로킹 당했습니다."), *SelfActor->GetName());

	PauseCard();
	RETURN_IF_INVALID(OtherActor);
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

	if (IPCREricaCardInteractable* EricaCardAttachableInterface = Cast<IPCREricaCardInteractable>(OtherActor))
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s와 충돌한 %s는 상호작용이 가능한 오브젝트입니다."), *SelfActor->GetName(), *OtherActor->GetName());
		EricaCardAttachableInterface->BindOnCardReturnBegin(this);
	}
}

/**
 * 카드를 플레이어 방향으로 이동시키고 플레이어에 닿을 시 풀로 반환합니다. 이때 모든 충돌판정을 무시합니다.
 */
void APCREricaCardProjectile::HandleCardReturn(float DeltaSeconds)
{
	RETURN_IF_INVALID(GetOwner());
	const FVector MoveDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	const FVector MoveVector = MoveDirection * CardReturnSpeed;
	const FRotator MoveRotator = FRotationMatrix::MakeFromX(MoveDirection).Rotator();

	if (GetBoxComponent())
	{
		GetBoxComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
	}

	SetActorLocationAndRotation(GetActorLocation() + (MoveVector * DeltaSeconds), MoveRotator);

	const float OwnerDistanceSquared = FVector::DistSquared(GetOwner()->GetActorLocation(), GetActorLocation());
	if (OwnerDistanceSquared <= (CardReleaseRange * CardReleaseRange))
	{
		CurrentCardState = ECardState::Invalid;
		ReleaseToProjectilePool();
	}
}

/**
 * 카드를 쏜 지점부터 특정 거리에 도달할 시 카드를 멈춥니다.
 */
void APCREricaCardProjectile::CheckCardRangeAndStop(float DeltaSeconds)
{
	const float Distance = FVector::DistSquared(ShootLocation, GetActorLocation());
	if (Distance >= (CardRange * CardRange))
	{
		PauseCard();
		CardRibbonFXComponent->Deactivate();
		HandleCardMaxRange();
	}
}

void APCREricaCardProjectile::HandleCardMaxRange()
{
	GetStaticMeshComponent()->SetVisibility(false);
	EnableCardFloatingFX();
}

void APCREricaCardProjectile::EnableCardFloatingFX()
{
	RETURN_IF_INVALID(CardFloatingFXComponent);
	CardFloatingFXComponent->SetVisibility(true);
	CardFloatingFXComponent->Activate();
}

void APCREricaCardProjectile::DisableCardFloatingFX()
{
	RETURN_IF_INVALID(CardFloatingFXComponent);
	CardFloatingFXComponent->SetVisibility(false);
	CardFloatingFXComponent->Deactivate();
}
