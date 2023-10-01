// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/Base/PCRBaseProjectile.h"

#include "Entities/Projectiles/Base/PCRProjectileDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

#include "Components/BoxComponent.h"
#include "Entities/Projectiles/Base/PCRBaseProjectilePool.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRBaseProjectile::APCRBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpeed = 1000.f;

	static ConstructorHelpers::FObjectFinder<UPCRProjectileDataAsset> DA_Projectile(TEXT("/Script/ProjectCardReturn.PCRProjectileDataAsset'/Game/DataAssets/DA_Projectile.DA_Projectile'"));
	if (DA_Projectile.Succeeded())
	{
		ProjectileDataAsset = DA_Projectile.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetupAttachment(BoxComponent);
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
		ProjectileMovementComponent->Deactivate();
	}
}

void APCRBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// SetCollision();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void APCRBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void APCRBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 투사체에 필요한 정보를 초기화 해줍니다. 투사체 사용전에 호출해줘야합니다. 하지만 대개 생성을 담당하는 풀에서 호출합니다.
 * @param Shooter 사용자의 포인터
 * @param Pool 풀의 포인터(풀을 사용시 넣어주세요)
 */
void APCRBaseProjectile::Init(AActor* Shooter, APCRBaseProjectilePool* Pool)
{
	SetOwner(Shooter);
	OwnerPool = Pool;
}

/**
 * 투사체를 발사합니다.
 * @param Direction 투사체의 발사 방향
 */
void APCRBaseProjectile::Shoot(const FVector& Direction)
{
	ShootLocation = GetOwner()->GetActorLocation();
	SetActorRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	GetProjectileMovementComponent()->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;
	
	OnShootCard.Broadcast();
}

void APCRBaseProjectile::SetCollision(bool bIsEnable)
{
	if (bIsEnable)
	{
		if (BoxComponent)
		{
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel3);
			BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		}
	}
	else
	{
		if (BoxComponent)
		{
			BoxComponent->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		}
	}
}

/**
 * 투사체 풀로 반환합니다.
 */
void APCRBaseProjectile::ReturnToProjectilePool()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	BoxComponent->SetCollisionProfileName("NoCollision");
	ProjectileMovementComponent->Deactivate();
	RETURN_IF_INVALID(IsValid(OwnerPool));
	OwnerPool->ReturnProjectile(this);
}
