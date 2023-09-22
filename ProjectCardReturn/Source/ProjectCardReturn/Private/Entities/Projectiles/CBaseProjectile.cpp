// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CBaseProjectile.h"

#include "Entities/Projectiles/CProjectileDataAsset.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

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
		ProjectileMovementComponent->InitialSpeed = 1000.f;
		ProjectileMovementComponent->MaxSpeed = 1000.f;
	}
}

void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}

void ACBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 투사체에 필요한 정보를 초기화 해줍니다. 투사체 사용전에 호출해줘야합니다. 하지만 보통 풀에서 생성 시 호출합니다.
 * @param Shooter 사용자의 포인터
 * @param Pool 풀의 포인터(풀을 사용시 넣어주세요)
 */
void ACBaseProjectile::Init(AActor* Shooter, AActor* Pool)
{
	SetOwner(Shooter);
	OwnerPool = Pool;
	ShootLocation = GetOwner()->GetActorLocation();
	SetActorTickEnabled(true);
}
