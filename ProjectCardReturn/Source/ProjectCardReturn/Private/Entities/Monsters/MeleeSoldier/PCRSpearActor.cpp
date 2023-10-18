// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRSpearActor.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "Components/BoxComponent.h"

APCRSpearActor::APCRSpearActor()
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
		StaticMeshComponent->SetStaticMesh(MonsterDataAsset->SpearMesh);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-1.52, -0.47, -52.35), FRotator(0.0, 90.0, 0.0));
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}
}

void APCRSpearActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSpearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSpearActor::DetachAndDelayedDestroy()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetCollisionProfileName("Ragdoll");

	OnDetachedSpear.Broadcast();

	RETURN_IF_INVALID(BoxComponent);
	BoxComponent->SetSimulatePhysics(true);

	DelayedDestroy();
}

void APCRSpearActor::DelayedDestroy()
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &APCRSpearActor::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, 3.f, false);
}

void APCRSpearActor::DestroyTimerCallback()
{
	Destroy();
}

