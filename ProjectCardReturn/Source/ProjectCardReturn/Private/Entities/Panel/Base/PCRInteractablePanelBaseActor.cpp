// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Panel/Base/PCRInteractablePanelBaseActor.h"

#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "Engine/DamageEvents.h"
#include "Entities/Panel/Base/PCRInteractablePanelDataAsset.h"

DEFINE_LOG_CATEGORY(PCRLogInteractablePanelBaseActor);

APCRInteractablePanelBaseActor::APCRInteractablePanelBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EState::Invalid;
	// TODO: 파라미터화 필요
	TippingTime = 0.2f;
	// TODO: 파라미터화 필요
	ElapsedTime = 0.f;
	// TODO: 파라미터화 필요
	StandUpCooldownTime = 1.f;

	static ConstructorHelpers::FObjectFinder<UPCRInteractablePanelDataAsset> DA_InteractablePanelDataAsset(TEXT("/Script/ProjectCardReturn.PCRInteractablePanelDataAsset'/Game/DataAssets/DA_InteractablePanel.DA_InteractablePanel'"));
	if (DA_InteractablePanelDataAsset.Succeeded())
	{
		MonsterDataAsset = DA_InteractablePanelDataAsset.Object;
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent && MonsterDataAsset)
	{
		RootComponent = StaticMeshComponent;
		StaticMeshComponent->SetStaticMesh(MonsterDataAsset->PanelMesh);
		StaticMeshComponent->SetCollisionProfileName("BlockPlayerProjectile");
	}
}

void APCRInteractablePanelBaseActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StartRotation = GetActorRotation();
	OnActorBeginOverlap.AddDynamic(this, &APCRInteractablePanelBaseActor::HandleBeginOverlap);
}

void APCRInteractablePanelBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

void APCRInteractablePanelBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
		case EState::Standing:
		{
			return;
		}
		case EState::Tipping:
		{
			HandleTipping(DeltaTime);
			return;
		}
		case EState::Tipped:
		{
			return;
		}
		case EState::StandingUp:
		{
			HandleStandingUp(DeltaTime);
			return;
		}
		case EState::Invalid:
		{
			return;
		}
	}
}

/**
 * 패널에 카드가 박히면 인터페이스를 통해 호출되며 이때 바인드하기 위한 함수입니다.
 * @param AttachedCard 패널에 박힌 카드
 */
void APCRInteractablePanelBaseActor::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	RETURN_IF_INVALID(AttachedCard);
	const FDelegateHandle NewHandle = AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRInteractablePanelBaseActor::HandleReturnCard);
	OnReturnCardBeginDelegateMap.Add(AttachedCard, NewHandle);
}

/**
 * 오버랩 처리를 담당합니다.
 * @param OverlappedActor 자신 액터
 * @param OtherActor 겹쳐진 상대 액터
 */
void APCRInteractablePanelBaseActor::HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// 만약 카드가 오버랩된 경우라면 박히지는 않았지만 카드가 복귀하던 중에 패널에 부딫힌 상태로 이 경우도 패널이 넘어지도록 하기 위한 코드입니다.
	APCREricaCardProjectile* OtherCard = Cast<APCREricaCardProjectile>(OtherActor);
	if (OtherCard)
	{
		HandleReturnCard(OtherCard);

		return;
	}

	UE_LOG(PCRLogInteractablePanelBaseActor, Log, TEXT("%s가 %s를 덮쳤습니다"), *OverlappedActor->GetName(), *OtherActor->GetName());

	// TODO: 현재 데미지 0을 스턴으로 활용하고 있지만 추후 인터페이스를 통해 전달하도록 수정 필요
	const FDamageEvent DamageEvent;
	OtherActor->TakeDamage(3.f, DamageEvent, nullptr, OverlappedActor);
}

/**
 * 넘어짐을 담당하는 함수입니다.
 */
void APCRInteractablePanelBaseActor::HandleTipping(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(ElapsedTime / TippingTime, 0.f, 1.f);
	const FRotator CurrentRotation = FMath::Lerp(StartRotation, EndRotation, Alpha);
	SetActorRotation(CurrentRotation);

	if (Alpha >= 1.f)
	{
		CurrentState = EState::Tipped;
		ElapsedTime = 0.f;
		SetActorTickEnabled(false);
		DisableCollisionDetection();

		FTimerHandle StandUpCooldownTimerHandle;
		FTimerDelegate StandUpCooldownTimerDelegate;
		StandUpCooldownTimerDelegate.BindUObject(this, &APCRInteractablePanelBaseActor::StandUpCooldownTimerCallback);
		GetWorldTimerManager().SetTimer(StandUpCooldownTimerHandle, StandUpCooldownTimerDelegate, StandUpCooldownTime, false);
	}
}

void APCRInteractablePanelBaseActor::StandUpCooldownTimerCallback()
{
	CurrentState = EState::StandingUp;
	SetActorTickEnabled(true);
}

/**
 * 일어남을 담당하는 함수입니다.
 */
void APCRInteractablePanelBaseActor::HandleStandingUp(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(ElapsedTime / TippingTime, 0.f, 1.f);
	const FRotator CurrentRotation = FMath::Lerp(EndRotation, StartRotation, Alpha);
	SetActorRotation(CurrentRotation);

	if (Alpha >= 1.f)
	{
		CurrentState = EState::Standing;
		ElapsedTime = 0.f;
		SetActorTickEnabled(false);
		EnableCollisionDetection();
	}
}

/**
 * 보통 카드가 복귀할때 호출되며 패널을 넘어지는 상태로 바꿉니다.
 * @param AttachedCard 박힌 카드
 */
void APCRInteractablePanelBaseActor::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	// 카드를 복귀시킬 시 오버랩 이벤트도 1번 추가로 발생하는데 이때 이 함수가 2번 호출되어도 1번만 동작하도록 하는 코드입니다.
	if (CurrentState == EState::Tipping)
	{
		return;
	}
	
	const FDelegateHandle* ExistingHandle = OnReturnCardBeginDelegateMap.Find(AttachedCard);
	if (ExistingHandle)
	{
		AttachedCard->OnReturnCardBegin.Remove(*ExistingHandle);
		// OnReturnCardBeginDelegateMap.Remove(AttachedCard);
	}

	UE_LOG(PCRLogInteractablePanelBaseActor, Log, TEXT("%s가 %s를 넘어뜨립니다."), *AttachedCard->GetName(), *this->GetName());

	CurrentState = EState::Tipping;
	SetActorTickEnabled(true);
	RETURN_IF_INVALID(StaticMeshComponent);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	const FVector CardReturnDirection = AttachedCard->GetOwner()->GetActorLocation() - AttachedCard->GetActorLocation();
	const float DotResult = FVector::DotProduct(GetActorForwardVector(), CardReturnDirection);
	if (DotResult >= 0)
	{
		EndRotation = FRotator(-90.0, GetActorRotation().Yaw, GetActorRotation().Roll);
	}
	else
	{
		EndRotation = FRotator(90.0, GetActorRotation().Yaw, GetActorRotation().Roll);
	}
}

/**
 * 콜리전을 활성화합니다.
 */
void APCRInteractablePanelBaseActor::EnableCollisionDetection()
{
	RETURN_IF_INVALID(StaticMeshComponent);
	StaticMeshComponent->SetCollisionProfileName("BlockPlayerProjectile");
}

/**
 * 콜리전을 비활성화합니다.
 */
void APCRInteractablePanelBaseActor::DisableCollisionDetection()
{
	RETURN_IF_INVALID(StaticMeshComponent);
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
}
