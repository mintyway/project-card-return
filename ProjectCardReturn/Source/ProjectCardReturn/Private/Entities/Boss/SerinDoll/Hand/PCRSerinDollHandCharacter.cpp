// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/PCRSerinDollHeadCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandCharacter);

APCRSerinDollHandCharacter::APCRSerinDollHandCharacter()
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	}
	
	if (GetMesh() && SerinDollDataAsset)
	{
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetRelativeScale3D(FVector(0.25, 0.25, 0.25));

		GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel9);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
		GetMesh()->SetGenerateOverlapEvents(true);
		
		GetMesh()->SetSkeletalMesh(SerinDollDataAsset->HandMesh);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (UClass* NewAnimInstanceClass = SerinDollDataAsset->AnimInstanceClass.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(NewAnimInstanceClass);
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		GetCharacterMovement()->bCheatFlying = true;
	}
	
}

void APCRSerinDollHandCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if WITH_EDITOR
	if (!GetWorld()->IsPlayInEditor())
	{
		return;
	}
#endif
	
	CachedSerinDollHandAnimInstance = Cast<UPCRSerinDollHandAnimInstance>(GetMesh()->GetAnimInstance());
	check(CachedSerinDollHandAnimInstance);
}

void APCRSerinDollHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollHandCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APCRSerinDollHandCharacter::SetSerinDollHead(APCRSerinDollHeadCharacter* NewSerinDollHead)
{
	CachedSerinDollHead = NewSerinDollHead;
}

float APCRSerinDollHandCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CachedSerinDollHead->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}
