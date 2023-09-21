// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/CBaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
