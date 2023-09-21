// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CBaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// static ConstructorHelpers::FObjectFinder<UCProjectileDataAsset> DA_Projectile(TEXT("Refference"));
	// if (DA_Projectile.Succeeded())
	// {
	// 	ProjectileDataAsset = DA_Projectile.Object;
	// }
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent)
	{
		RootComponent = StaticMeshComponent;
	}
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 1000.f;
		ProjectileMovementComponent->MaxSpeed = 1000.f;
	}
}

void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
