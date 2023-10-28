// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/Base/PCRBaseProjectile.h"

#include "SimpleElementShaders.h"
#include "Entities/Projectiles/Base/PCRProjectileDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRBaseProjectile::APCRBaseProjectile() : 	ProjectileSpeed(1000.f)
{
	PrimaryActorTick.bCanEverTick = true;

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
	}
}

void APCRBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorHiddenInGame(true);
	DisableProjectile();
}

/**
 * 투사체를 활성화하고 발사합니다. 
 * @param NewOwner 이 투사체를 사용하는 객체의 포인터
 * @param StartLocation 투사체의 시작 위치 
 * @param Direction 투사체의 발사 방향
 */
void APCRBaseProjectile::LaunchProjectile(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction)
{
	SetOwner(NewOwner);
	ShootLocation = StartLocation;
	
	SetActorLocationAndRotation(ShootLocation, FRotationMatrix::MakeFromX(Direction).Rotator());
	SetActorHiddenInGame(false);
	EnableProjectile();
	ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;

	OnLaunchProjectile.Broadcast();
}

/**
 * 투사체를 활성화합니다.
 */
void APCRBaseProjectile::EnableProjectile()
{
	SetActorTickEnabled(true);
	EnableCollisionDetection();
	RETURN_IF_INVALID(ProjectileMovementComponent);
	ProjectileMovementComponent->Activate();
}

/**
 * 투사체를 비활성화합니다.
 */
void APCRBaseProjectile::DisableProjectile()
{
	SetActorTickEnabled(false);
	DisableCollisionDetection();
	RETURN_IF_INVALID(ProjectileMovementComponent);
	ProjectileMovementComponent->Deactivate();
}

/**
 * 투사체의 충돌판정을 비활성화합니다.
 */
void APCRBaseProjectile::DisableCollisionDetection()
{
	if (BoxComponent)
	{
		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

/**
 * 투사체 풀로 반환합니다.
 */
void APCRBaseProjectile::ReleaseToProjectilePool()
{
	SetActorHiddenInGame(true);
	DisableProjectile();
	
	OnReleaseProjectile.Broadcast(this);
}
