// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/PCRSpearActor.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "Components/BoxComponent.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Game/PCRGameModeBase.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSpearActor);

APCRSpearActor::APCRSpearActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Thrown = false;
	
	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
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
		BoxComponent->SetBoxExtent(FVector(1.5, 36.5, 52.0));
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel7);
		BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent && MonsterDataAsset)
	{
		StaticMeshComponent->SetupAttachment(BoxComponent);
		StaticMeshComponent->SetStaticMesh(MonsterDataAsset->SpearMesh);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-1.52, -0.47, -52.35), FRotator(0.0, 90.0, 0.0));
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = ParameterDataAsset->SpearSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
		ProjectileMovementComponent->Deactivate();
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

void APCRSpearActor::BeginDestroy()
{
	Super::BeginDestroy();
	
	OnDetachedSpear.Clear();
	OnDestroyedSpear.Clear();
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

void APCRSpearActor::Throw(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction)
{
	if (!Thrown)
	{
		SetOwner(NewOwner);

		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		const FVector ShootLocation = StartLocation;
		const FRotator ShooRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	
		SetActorLocationAndRotation(ShootLocation, ShooRotation);
		ProjectileMovementComponent->Activate();
		ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ParameterDataAsset->SpearSpeed; 

		BoxComponent->OnComponentHit.AddDynamic(this, &APCRSpearActor::HandleSpearHit);
		
		Thrown = true;
		
		DelayedDestroy();
	}
}

void APCRSpearActor::DelayedDestroy()
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &APCRSpearActor::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, 3.f, false);
}

void APCRSpearActor::HandleSpearHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (APCREricaCharacter* Erica = Cast<APCREricaCharacter>(OtherActor))
		{
			DestroyTimerCallback();
		}
	}
}

/*void APCRSpearActor::HandleBlockedByCard(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(PCRLogSpearActor, Warning, TEXT("%s"), *FString(__FUNCTION__));
	ProjectileMovementComponent->Deactivate();
	DelayedDestroy();
}*/

/*void APCRSpearActor::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	AttachedCard->OnActorHit.AddDynamic(this, &APCRSpearActor::HandleBlockedByCard);
}*/

void APCRSpearActor::DestroyTimerCallback()
{
	OnDestroyedSpear.Broadcast();
	
	Destroy();
}

