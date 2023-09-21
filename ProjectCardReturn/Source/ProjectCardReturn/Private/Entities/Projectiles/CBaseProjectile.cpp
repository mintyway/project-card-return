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

	Shooter = nullptr;
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

void ACBaseProjectile::Init(AActor* InitShooter)
{
	Shooter = InitShooter;
	StartLocation = Shooter->GetActorLocation();
	SetActorTickEnabled(true);
}
