// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "FMODBlueprintStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/Base/PCRProjectileDataAsset.h"
#include "Game/PCRParameterDataAsset.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "Entities/Panel/Base/PCRInteractablePanelBaseActor.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCardProjectile);

APCREricaCardProjectile::APCREricaCardProjectile() : ForwardDamage(0.f), BackwardDamage(0.f), CurrentCardState(ECardState::Invalid)
{
	InitCollisionObjectQueryParams();
	
	if (ParameterDataAsset)
	{
		ProjectileSpeed = ParameterDataAsset->EricaCardSpeed;
		CardReturnSpeed = ParameterDataAsset->EricaCardReturnSpeed;
		CardRange = ParameterDataAsset->EricaNarrowShotRange;
		KnockBackPower = ParameterDataAsset->EricaCardKnockBackPower;
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

	CardShotRibbonFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardShotRibbonFXComponent"));
	if (CardShotRibbonFXComponent && ProjectileDataAsset)
	{
		CardShotRibbonFXComponent->SetupAttachment(BoxComponent);
		CardShotRibbonFXComponent->SetAsset(ProjectileDataAsset->ShotCardRibbon);
	}

	CardRecallRibbonFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CardRecallRibbonFXComponent"));
	if (CardRecallRibbonFXComponent)
	{
		CardRecallRibbonFXComponent->SetupAttachment(BoxComponent);
		CardRecallRibbonFXComponent->SetAsset(ProjectileDataAsset->RecallCardRibbon);
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

#if WITH_EDITOR
	SetFolderPath(TEXT("Erica/Cards"));
#endif

	OnActorBeginOverlap.AddDynamic(this, &APCREricaCardProjectile::HandleBeginOverlap);
	OnActorHit.AddDynamic(this, &APCREricaCardProjectile::HandleBlocking);

	check(CardShotRibbonFXComponent && CardFloatingFXComponent);
	CardShotRibbonFXComponent->Deactivate();
	CardRecallRibbonFXComponent->Deactivate();
	CardFloatingFXComponent->Deactivate();
}

void APCREricaCardProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LastTickLocation = GetActorLocation();
	LastTickForwardDirection = GetActorForwardVector();

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
	CardShotRibbonFXComponent->Activate(true);

	CurrentCardState = ECardState::Flying;
}

void APCREricaCardProjectile::ReleaseToProjectilePool()
{
	Super::ReleaseToProjectilePool();

	AttackedActors.Reset();
	CardShotRibbonFXComponent->Deactivate();
	CardRecallRibbonFXComponent->Deactivate();
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

	const FVector Direction = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	SetActorRotation(Rotation);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EnableProjectile();
	AttackedActors.Reset();

	CardRecallRibbonFXComponent->Activate(true);
	CardFloatingFXComponent->Deactivate();
	StaticMeshComponent->SetVisibility(true);

	// 겹친 상태를 유지하고 있는 대상이 있는 경우 즉시 데미지를 주기 위한 코드입니다. 만약 이 코드가 없다면 카드 복귀 시 BeginOverlap이 발생하지 않아 데미지가 들어가지 않습니다.
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (const auto& OverlappingActor : OverlappingActors)
	{
		HandleBeginOverlap(this, OverlappingActor);
	}

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
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECR_Overlap);
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
	// 이미 맞은 상태면 다시 공격되지 않도록 하는 코드입니다.
	if (AttackedActors.Find(OtherActor) != INDEX_NONE)
	{
		return;
	}

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
	
	const FVector CurrentDirection = LastTickForwardDirection;
	const FVector CurrentOtherActorDirection = OtherCharacter->GetActorForwardVector();
	const float DotResult = FVector::DotProduct(CurrentDirection, CurrentOtherActorDirection);
	if (DotResult <= 0)
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 %s와 정면으로 충돌했습니다."), *OverlappedActor->GetName(), *OtherActor->GetName());

		// 만약 대상이 몬스터인 경우 넉백을 발생시키는 코드입니다.
		if (APCRMonsterBaseCharacter* MonsterCharacter = Cast<APCRMonsterBaseCharacter>(OtherCharacter))
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

	AttackedActors.Add(OtherActor);
}

void APCREricaCardProjectile::HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileDataAsset)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileDataAsset->CardBlockedHit, GetActorLocation(), GetActorRotation());
	}

	const FTransform BlockedCardTransform = FTransform(GetActorRotation(), GetActorLocation());
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), SoundDataAsset->BlockedCard, BlockedCardTransform, true);

	UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s 카드가 블로킹 당했습니다."), *SelfActor->GetName());

	PauseCard();
	check(OtherActor);
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

	if (IPCREricaCardInteractable* EricaCardAttachableInterface = Cast<IPCREricaCardInteractable>(OtherActor))
	{
		UE_LOG(PCRLogEricaCardProjectile, Log, TEXT("%s와 충돌한 %s는 상호작용이 가능한 오브젝트입니다."), *SelfActor->GetName(), *OtherActor->GetName());
		EricaCardAttachableInterface->BindOnReturnCardBegin(this);
	}
}

/**
 * 카드를 플레이어 방향으로 이동시키고 플레이어에 닿을 시 풀로 반환합니다. 이때 모든 충돌판정을 무시합니다.
 */
void APCREricaCardProjectile::HandleCardReturn(float DeltaSeconds)
{
	check(GetOwner());
	const FVector MoveDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	const FVector MoveVector = MoveDirection * CardReturnSpeed;
	const FRotator MoveRotator = FRotationMatrix::MakeFromX(MoveDirection).Rotator();

	if (BoxComponent)
	{
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Overlap);
	}

	const FVector CurrentTickLocation = GetActorLocation() + (MoveVector * DeltaSeconds);
	bool bShouldRelease = false;
	
	TArray<FHitResult> HitResults;
	if (GetWorld()->LineTraceMultiByObjectType(HitResults, LastTickLocation, CurrentTickLocation, CollisionObjectQueryParams))
	{
		for (const auto& TestResult : HitResults)
		{
			if (Cast<APCREricaCharacter>(TestResult.GetActor()))
			{
				bShouldRelease = true;
			}
			else if (Cast<APCRMonsterBaseCharacter>(TestResult.GetActor()))
			{
				HandleBeginOverlap(this, TestResult.GetActor());
			}
			else if (Cast<APCRInteractablePanelBaseActor>(TestResult.GetActor()))
			{
				HandleBeginOverlap(this, TestResult.GetActor());
				if (APCRInteractablePanelBaseActor* Panel = Cast<APCRInteractablePanelBaseActor>(TestResult.GetActor()))
				{
					Panel->HandleBeginOverlap(Panel, this);
				}
			}
		}
	}

	DrawDebugLine(GetWorld(), LastTickLocation, CurrentTickLocation, FColor::Green, false, 3, 0, 1);

	if (bShouldRelease)
	{
		CurrentCardState = ECardState::Invalid;
		ReleaseToProjectilePool();
	}
	else
	{
		SetActorLocationAndRotation(CurrentTickLocation, MoveRotator);
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
		CardShotRibbonFXComponent->Deactivate();
		HandleCardMaxRange();
	}
}

void APCREricaCardProjectile::HandleCardMaxRange()
{
	StaticMeshComponent->SetVisibility(false);
	CardFloatingFXComponent->Activate(true);
}

void APCREricaCardProjectile::InitCollisionObjectQueryParams()
{
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);
}
