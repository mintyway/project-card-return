// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

UPCRMonsterBaseAnimInstance::UPCRMonsterBaseAnimInstance(): bShouldMove(false)
{
	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_MonsterDataAsset(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_MonsterDataAsset.Succeeded())
	{
		MonsterDataAsset = DA_MonsterDataAsset.Object;
	}
}

void UPCRMonsterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedMonsterBaseCharacter = Cast<APCRMonsterBaseCharacter>(TryGetPawnOwner());
	if (!CachedMonsterBaseCharacter)
	{
		NULL_POINTER_EXCEPTION(CachedMonsterBaseCharacter);
	}
	
	CachedCharacterMovement = CachedMonsterBaseCharacter ? Cast<UCharacterMovementComponent>(CachedMonsterBaseCharacter->GetCharacterMovement()) : nullptr;
	if (!CachedCharacterMovement)
	{
		NULL_POINTER_EXCEPTION(CachedCharacterMovement);
	}
}

void UPCRMonsterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CachedCharacterMovement)
	{
		Velocity = CachedCharacterMovement->Velocity;
		bShouldMove = Velocity.SizeSquared2D() >= FMath::Square(30);
	}
}