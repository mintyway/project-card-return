// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Projectile/PCRSerinDollPattern1Projectile.h"

#include "FMODBlueprintStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRSerinDollPattern1Projectile::APCRSerinDollPattern1Projectile()
	: State(ESerinDollProjectileState::Unused), ProjectileSpeed(5000.f), ProjectileReturnSpeed(10000.f), Range(3000.f), bOnceDetached(false)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRSerinDollPrimaryDataAsset> DA_SerinDoll(TEXT("/Script/ProjectCardReturn.PCRSerinDollPrimaryDataAsset'/Game/DataAssets/DA_SerinDoll.DA_SerinDoll'"));
	if (DA_SerinDoll.Succeeded())
	{
		SerinDollDataAsset = DA_SerinDoll.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	if (SphereComponent)
	{
		SetRootComponent(SphereComponent);
		SphereComponent->InitSphereRadius(50.f);
		SphereComponent->SetCollisionProfileName("NoCollision");
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetupAttachment(SphereComponent);
		StaticMeshComponent->SetRelativeScale3D(FVector(2.f));
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
		StaticMeshComponent->SetStaticMesh(SerinDollDataAsset->Pattern1ProjectileMesh);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	}

	Pattern1ThrowEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pattern1ThrowEffectComponent"));
	if (Pattern1ThrowEffectComponent && SerinDollDataAsset)
	{
		Pattern1ThrowEffectComponent->SetupAttachment(GetRootComponent());
		Pattern1ThrowEffectComponent->SetAsset(SerinDollDataAsset->Pattern1ThrowEffect);
	}
}

void APCRSerinDollPattern1Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DisableProjectile();
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APCRSerinDollPattern1Projectile::HandleBossOverlap);
}

void APCRSerinDollPattern1Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
		case ESerinDollProjectileState::Unused:
		{
			return;
		}
		case ESerinDollProjectileState::Shooting:
		{
			HandleShooting();
			break;
		}
		case ESerinDollProjectileState::Stop:
		{
			break;
		}
		case ESerinDollProjectileState::Returning:
		{
			break;
		}
	}
}

void APCRSerinDollPattern1Projectile::Shoot(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction)
{
	SetOwner(NewOwner);
	LaunchLocation = StartLocation;
	
	UFMODAudioComponent* AudioComponent = UFMODBlueprintStatics::PlayEventAttached(SoundDataAsset->Pattern1BombThrow, GetRootComponent(), NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, false, false, true);

	AudioComponent->Play();
	
	SetActorLocationAndRotation(LaunchLocation, FRotationMatrix::MakeFromX(Direction).Rotator());
	SetActorHiddenInGame(false);
	EnableProjectile();
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;

	State = ESerinDollProjectileState::Shooting;
}

void APCRSerinDollPattern1Projectile::Release()
{
	Destroy();
}

void APCRSerinDollPattern1Projectile::Pattern1ExplosionTimerStart()
{
	// TODO: 타이머 파라미터화 필요
	const float Time = 10.f;
	const float TimerSoundTime = 3.f;

	FTimerHandle TimerSoundHandle;
	GetWorldTimerManager().SetTimer(TimerSoundHandle, FTimerDelegate::CreateUObject(this, &APCRSerinDollPattern1Projectile::HandlePattern1ExplosionTimerSound), Time - TimerSoundTime, false);

	FTimerHandle HandlePattern1ExplosionTimerHandle;
	GetWorldTimerManager().SetTimer(HandlePattern1ExplosionTimerHandle, FTimerDelegate::CreateUObject(this, &APCRSerinDollPattern1Projectile::HandlePattern1Explosion), Time, false);
}

void APCRSerinDollPattern1Projectile::BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard)
{
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRSerinDollPattern1Projectile::HandlePattern1DetachedCard);
}

void APCRSerinDollPattern1Projectile::EnableProjectile()
{
	ProjectileMovementComponent->Activate();
	Pattern1ThrowEffectComponent->Activate(true);
	SetActorHiddenInGame(false);
	EnableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::DisableProjectile()
{
	ProjectileMovementComponent->Deactivate();
	Pattern1ThrowEffectComponent->Deactivate();
	SetActorHiddenInGame(true);
	DisableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::EnableCollisionDetection()
{
	StaticMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel6);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
}

void APCRSerinDollPattern1Projectile::DisableCollisionDetection()
{
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void APCRSerinDollPattern1Projectile::HandleShooting()
{
	if (IsAtMaxRange())
	{
		EnableProjectile();
		HandleStop();
	}
}

bool APCRSerinDollPattern1Projectile::IsAtMaxRange()
{
	const float Distance = FVector::Dist(LaunchLocation, GetActorLocation());
	if (Distance >= Range)
	{
		return true;
	}

	return false;
}

void APCRSerinDollPattern1Projectile::HandleStop()
{
	ProjectileMovementComponent->Deactivate();
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);

	State = ESerinDollProjectileState::Stop;
}

void APCRSerinDollPattern1Projectile::HandlePattern1DetachedCard(APCREricaCardProjectile* AttachedCard)
{
	if (!bOnceDetached)
	{
		const APCRSerinDollHeadCharacter* CachedSerinDollHead = Cast<APCRSerinDollHeadCharacter>(GetOwner());
		check(CachedSerinDollHead);

		const APCRLiftActor* CachedLift = CachedSerinDollHead->GetCachedLift();
		check(CachedLift);

		FVector TargetLocation;
		FVector Direction;
		if (CachedLift->IsOverlappedPattern1())
		{
			TargetLocation = CachedSerinDollHead->GetActorLocation();
			TargetLocation.Z = GetActorLocation().Z;
			Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		}
		else
		{
			TargetLocation = CachedSerinDollHead->GetCachedErica()->GetActorLocation();
			TargetLocation.Z = GetActorLocation().Z;
			Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		}

		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECR_Overlap);
		ProjectileMovementComponent->Activate();
		ProjectileMovementComponent->Velocity = Direction * ProjectileReturnSpeed;
		const FRotator NewRotator = FRotationMatrix::MakeFromX(ProjectileMovementComponent->Velocity.GetSafeNormal()).Rotator();
		SetActorRotation(NewRotator);
		Pattern1ThrowEffectComponent->Activate(true);

		if (OnDetachedCard.IsBound())
		{
			OnDetachedCard.Execute();
		}

		State = ESerinDollProjectileState::Returning;

		bOnceDetached = true;
	}
}

void APCRSerinDollPattern1Projectile::HandlePattern1Explosion()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
	                                               SerinDollDataAsset->Pattern1BombEffect, GetActorLocation());

	const FTransform NewTransform = FTransform(GetActorRotation(), GetActorLocation());
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), SoundDataAsset->Pattern1Bomb, NewTransform, true);

	TArray<FOverlapResult> OutOverlaps;
	const float Radius = 700.f;
	const float Damage = 15.f;
	const bool bSucceed = GetWorld()->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius));
	if (bSucceed)
	{
		for (const auto& OutOverlap : OutOverlaps)
		{
			if (AActor* OverlappedActor = OutOverlap.GetActor())
			{
				FDamageEvent DamageEvent;
				const APCRSerinDollHeadCharacter* SerinDollHead = Cast<APCRSerinDollHeadCharacter>(GetOwner());
				check(SerinDollHead);
				OverlappedActor->TakeDamage(Damage, DamageEvent, SerinDollHead->GetController(), this);
			}
		}
	}

	const FColor Color = bSucceed ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, Color,
	                false, 1.f);

	Destroy();
}

void APCRSerinDollPattern1Projectile::HandlePattern1ExplosionTimerSound()
{
	const FTransform NewTransform = FTransform(GetActorRotation(), GetActorLocation());
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), SoundDataAsset->Pattern1BombTimer, NewTransform, true);
}

void APCRSerinDollPattern1Projectile::HandleBossOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float DamageAmount;
	const FDamageEvent DamageEvent;

	if (Cast<APCREricaCharacter>(OtherActor))
	{
		DamageAmount = 20.f;
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
	}
	else
	{
		DamageAmount = 5.f;
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
											   SerinDollDataAsset->Pattern1BombEffect, GetActorLocation());
		
		const FTransform NewTransform = FTransform(GetActorRotation(), GetActorLocation());
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), SoundDataAsset->Pattern1BombCrash, NewTransform, true);

		Destroy();
	}
}
