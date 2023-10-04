// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(PCRLogShieldActor);

APCRShieldActor::APCRShieldActor()
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
		BoxComponent->SetBoxExtent(FVector(15.0, 50.0, 50.0));
		BoxComponent->SetCollisionProfileName("BlockPlayerProjectile");
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent && MonsterDataAsset)
	{
		StaticMeshComponent->SetupAttachment(BoxComponent);
		StaticMeshComponent->SetStaticMesh(MonsterDataAsset->ShieldMesh);
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}
}

void APCRShieldActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorHit.AddDynamic(this, &APCRShieldActor::HandleBlocking);

	RETURN_IF_INVALID(BoxComponent);
	BoxComponent->SetMassOverrideInKg(EName::None, 30.f, true);
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

	OnDetachedShield.RemoveAll(this);
	OnDetachedCard.RemoveAll(this);
}

/**
 * 주인으로부터 실드를 탈착하고 물리 시뮬레이션을 활성화합니다. 이후 파괴합니다.\n
 * 물리 시뮬레이션을 활성화하는 이유는 실드가 바닥에서 뒹굴 수 있도록 하기 위함입니다.
 */
void APCRShieldActor::DetachAndDelayedDestroy()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	OnDetachedShield.Broadcast();

	RETURN_IF_INVALID(BoxComponent);
	BoxComponent->SetSimulatePhysics(true);

	DelayedDestroy();
}

/**
 * 액터가 서로 블로킹(충돌) 될 시 발생하는 이벤트의 처리를 담당합니다.\n
 * 여기서는 충돌된 액터가 카드인지 확인하고 카드라면 접근하여 OnReturnBegin 델리게이트에 바인드합니다.\n
 * 바인드의 이유는 카드의 회수가 시작되는 타이밍에 방패에 특정 상호작용을 하기 위함입니다.
 * @param SelfActor 자신
 * @param OtherActor 충돌한 액터
 * @param NormalImpulse 충돌한 힘
 * @param Hit 충돌결과
 */
void APCRShieldActor::HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APCREricaCardProjectile* AttachedCard = Cast<APCREricaCardProjectile>(OtherActor))
	{
		const FDelegateHandle NewHandle = AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRShieldActor::HandleReturnCard);
		OnReturnCardBeginDelegateMap.Add(AttachedCard, NewHandle);
	}
}

/**
 * 호출되면 일정 시간 후에 객체를 파괴합니다.
 */
void APCRShieldActor::DelayedDestroy()
{
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, FTimerDelegate::CreateLambda([this]() -> void
	{
		Destroy();
	}), 3.f, false);
}

/**
 * 
 * @param AttachedCard 카드의 회수가 시작되는 시점
 */
void APCRShieldActor::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	const FDelegateHandle* ExistingHandle = OnReturnCardBeginDelegateMap.Find(AttachedCard);
	if (ExistingHandle)
	{
		AttachedCard->OnReturnCardBegin.Remove(*ExistingHandle);
	}

	DetachAndDelayedDestroy();

	const FVector Direction = (AttachedCard->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float ImpulseSize = 5000.f;
	BoxComponent->AddImpulse(Direction * ImpulseSize);
	UE_LOG(PCRLogShieldActor, Log, TEXT("%s가 당겨집니다."), *GetName());
}
