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

void APCRShieldActor::HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APCREricaCardProjectile* AttachedCard = Cast<APCREricaCardProjectile>(OtherActor))
	{
		const FDelegateHandle NewHandle = AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRShieldActor::HandleReturnCard);
		OnReturnCardBeginDelegateMap.Add(AttachedCard, NewHandle);

	}
}

void APCRShieldActor::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	const FDelegateHandle* ExistingHandle = OnReturnCardBeginDelegateMap.Find(AttachedCard);
	if (ExistingHandle)
	{
		AttachedCard->OnReturnCardBegin.Remove(*ExistingHandle);
	}

	OnDetachedCard.Broadcast();

	const FVector Direction = (AttachedCard->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float ImpulseSize = 5000.f;
	BoxComponent->AddImpulse(Direction * ImpulseSize);
	UE_LOG(PCRLogShieldActor, Log, TEXT("%s가 당겨집니다."), *GetName());
}
