// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(PCRLogShieldActor);

APCRShieldActor::APCRShieldActor() : AttachedCardCount(0)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetBoxExtent(FVector(1.5, 36.5, 52.0));
		BoxComponent->SetCollisionProfileName("BlockPlayerProjectile");
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent && MonsterDataAsset)
	{
		StaticMeshComponent->SetupAttachment(BoxComponent);
		StaticMeshComponent->SetStaticMesh(MonsterDataAsset->ShieldMesh);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-1.52, -0.47, -52.35), FRotator(0.0, 90.0, 0.0));
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}
}

void APCRShieldActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RETURN_IF_INVALID(BoxComponent);
	BoxComponent->SetMassOverrideInKg(EName::None, 3.f, true);
}

void APCRShieldActor::BeginPlay()
{
	Super::BeginPlay();
}

void APCRShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRShieldActor::BeginDestroy()
{
	Super::BeginDestroy();

	OnDetachedShield.Clear();
	OnDetachedCard.Clear();
}

/**
 * 방패가 카드에 박힌경우 카드의 델리게이트에 바인드하기위해 인터페이스를 통해 호출됩니다. 
 * @param AttachedCard 방패에 박힌 카드
 */
void APCRShieldActor::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	check(AttachedCard);
	++AttachedCardCount;
	const FDelegateHandle NewHandle = AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRShieldActor::HandleReturnCard);
	OnReturnCardBeginDelegateMap.Add(AttachedCard, NewHandle);
}

/**
 * 주인으로부터 실드를 탈착하고 물리 시뮬레이션을 활성화합니다. 이후 파괴합니다.\n
 * 물리 시뮬레이션을 활성화하는 이유는 실드가 바닥에서 뒹굴 수 있도록 하기 위함입니다.
 */
void APCRShieldActor::DetachAndDelayedDestroy()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetCollisionProfileName("Ragdoll");

	OnDetachedShield.Broadcast();

	RETURN_IF_INVALID(BoxComponent);
	BoxComponent->SetSimulatePhysics(true);

	DelayedDestroy();
}

/**
 * 호출되면 일정 시간 후에 객체를 파괴합니다.
 */
void APCRShieldActor::DelayedDestroy()
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &APCRShieldActor::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, 3.f, false);
}

/**
 * 카드가 복귀할때 처리를 담당합니다.
 * @param AttachedCard 방패에 박힌 카드
 */
void APCRShieldActor::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	const FDelegateHandle* ExistingHandle = OnReturnCardBeginDelegateMap.Find(AttachedCard);
	if (ExistingHandle)
	{
		AttachedCard->OnReturnCardBegin.Remove(*ExistingHandle);
		// OnReturnCardBeginDelegateMap.Remove(AttachedCard);
	}

	const int32 DetachCount = 2;
	if (AttachedCardCount >= DetachCount)
	{
		DetachAndDelayedDestroy();
		const FVector Direction = (AttachedCard->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const float ImpulseSize = 1000.f;
		BoxComponent->AddImpulse(Direction * ImpulseSize);
		UE_LOG(PCRLogShieldActor, Log, TEXT("%s가 당겨집니다."), *GetName());
	}

	AttachedCardCount = 0;
}

void APCRShieldActor::DestroyTimerCallback()
{
	Destroy();
}
