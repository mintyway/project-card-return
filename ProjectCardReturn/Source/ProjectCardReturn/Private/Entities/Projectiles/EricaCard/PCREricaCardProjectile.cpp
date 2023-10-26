// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/Base/PCRProjectileDataAsset.h"
#include "Game/PCRParameterDataAsset.h"
#include "Interfaces/PCREricaCardInteractable.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCardProjectile);

APCREricaCardProjectile::APCREricaCardProjectile() : ForwardDamage(0.f), BackwardDamage(0.f), CurrentCardState(ECardState::Invalid)
{
	if (ParameterDataAsset)
	{
		ProjectileSpeed = ParameterDataAsset->EricaCardSpeed;
		CardReturnSpeed = ParameterDataAsset->EricaCardReturnSpeed;
		CardRange = ParameterDataAsset->EricaNarrowShotRange;
		KnockBackPower = ParameterDataAsset->EricaCardKnockBackPower;
		CardReleaseRange = ParameterDataAsset->EricaCardReleaseRange;
	}

	if (BoxComponent)
	{
		BoxComponent->InitBoxExtent(FVector(30.8, 21.7, 1.0));
	}

	if (StaticMeshComponent && ProjectileDataAsset)
	{
		StaticMeshComponent->SetStaticMesh(ProjectileDataAsset->EricaCardMesh);
		StaticMeshComponent->SetRelativeScale3D(FVector(7.0, 7.0, 1.0));
	}

	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	}

	CardRibbonFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardRibbonFXComponent"));
	if (CardRibbonFXComponent && ProjectileDataAsset)
	{
		CardRibbonFXComponent->SetupAttachment(BoxComponent);
		CardRibbonFXComponent->SetAsset(ProjectileDataAsset->CardRibbon);
	}

	CardFloatingFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardFloatingFXComponent"));
	if (CardFloatingFXComponent && ProjectileDataAsset)
	{
		CardFloatingFXComponent->SetupAttachment(BoxComponent);
		CardFloatingFXComponent->SetAsset(ProjectileDataAsset->CardFloating);
	}
}

void APCREricaCardProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorBeginOverlap.AddDynamic(this, &APCREricaCardProjectile::HandleBeginOverlap);
	OnActorHit.AddDynamic(this, &APCREricaCardProjectile::HandleBlocking);

	check(CardRibbonFXComponent && CardFloatingFXComponent);
	CardRibbonFXComponent->Deactivate();
	CardFloatingFXComponent->Deactivate();
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
	CardRibbonFXComponent->Activate(true);

	CurrentCardState = ECardState::Flying;
}

void APCREricaCardProjectile::ReleaseToProjectilePool()
{
	Super::ReleaseToProjectilePool();

	AttackedCharacter.Reset();
	CardRibbonFXComponent->Deactivate();
	CardFloatingFXComponent->Deactivate();
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
	AttackedCharacter.Reset();
	CardRibbonFXComponent->Activate();
	CardFloatingFXComponent->Deactivate();
	StaticMeshComponent->SetVisibility(true);

	OnReturnCardBegin.Broadcast(this);
}

void APCREricaCardProjectile::SetDamage(float InForwardDamage, float InBackwardDamage)
{
	ForwardDamage = InForwardDamage;
	BackwardDamage = InBackwardDamage;
}

void APCREricaCardProjectile::EnableCollisionDetection()
{
	if (BoxComponent)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel3);
		BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Block);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECR_Block);
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
	if (ProjectileDataAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileDataAsset->CardPenetratedHit, GetActorLocation(), GetActorRotation());
	}

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (!OtherCharacter)
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s가 %s와 충돌했지만 %s이 물체이기 때문에 데미지를 주지 못했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName(), *OtherActor->GetName());

		return;
	}

	// 이미 맞은 상태면 다시 공격되지 않도록 하는 코드입니다.
	if (AttackedCharacter.Find(OtherCharacter) != INDEX_NONE)
	{
		return;
	}

	const FVector CurrentDirection = OverlappedActor->GetActorForwardVector();
	const FVector CurrentOtherActorDirection = OtherCharacter->GetActorForwardVector();
	const float DotResult = FVector::DotProduct(CurrentDirection, CurrentOtherActorDirection);
	if (DotResult <= 0)
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 %s와 정면으로 충돌했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName());

		// 만약 대상이 몬스터인 경우 넉백을 발생시키는 코드입니다.
		APCRMonsterBaseCharacter* MonsterCharacter = Cast<APCRMonsterBaseCharacter>(OtherCharacter);
		if (MonsterCharacter)
		{
			MonsterCharacter->LaunchCharacter(OverlappedActor->GetActorForwardVector() * KnockBackPower, true, false);
		}

		const APCREricaCharacter* EricaCharacter = Cast<APCREricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherCharacter->TakeDamage(ForwardDamage, DamageEvent, EricaCharacter->GetController(), this);
	}
	else
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 %s와 후면으로 충돌했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName());
		APCREricaCharacter* EricaCharacter = Cast<APCREricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(BackwardDamage, DamageEvent, EricaCharacter->GetController(), EricaCharacter);
	}

	AttackedCharacter.Add(OtherCharacter);
}

void APCREricaCardProjectile::HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileDataAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileDataAsset->CardBlockedHit, GetActorLocation(), GetActorRotation());
	}

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

	if (BoxComponent)
	{
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
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
	StaticMeshComponent->SetVisibility(false);
	CardFloatingFXComponent->Activate(true);
}
