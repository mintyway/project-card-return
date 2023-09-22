// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CBaseProjectile.h"

#include "Entities/Projectiles/CProjectileDataAsset.h"

#include "Components/BoxComponent.h"
#include "Entities/Projectiles/CBaseProjectilePool.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpeed = 1000.f;

	static ConstructorHelpers::FObjectFinder<UCProjectileDataAsset> DA_Projectile(TEXT("/Script/ProjectCardReturn.CProjectileDataAsset'/Game/DataAssets/DA_Projectile.DA_Projectile'"));
	if (DA_Projectile.Succeeded())
	{
		ProjectileDataAsset = DA_Projectile.Object;
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetCollisionProfileName("NoCollision");
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
		ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
		ProjectileMovementComponent->Deactivate();
	}
}

void ACBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 투사체에 필요한 정보를 초기화 해줍니다. 투사체 사용전에 호출해줘야합니다. 하지만 대개 생성을 담당하는 풀에서 호출합니다.
 * @param Shooter 사용자의 포인터
 * @param Pool 풀의 포인터(풀을 사용시 넣어주세요)
 */
void ACBaseProjectile::Init(AActor* Shooter, ACBaseProjectilePool* Pool)
{
	SetOwner(Shooter);
	OwnerPool = Pool;
}

/**
 * 투사체를 발사합니다.
 * @param Direction 투사체의 발사 방향
 */
void ACBaseProjectile::Shoot(const FVector& Direction)
{
	ShootLocation = GetOwner()->GetActorLocation();
	GetProjectileMovementComponent()->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;
}

/**
 * 투사체 풀로 반환합니다.
 */
void ACBaseProjectile::ReturnToProjectilePool()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	BoxComponent->SetCollisionProfileName("NoCollision");
	ProjectileMovementComponent->Deactivate();
	RETURN_IF_INVALID(IsValid(OwnerPool));
	OwnerPool->ReturnProjectile(this);
}
