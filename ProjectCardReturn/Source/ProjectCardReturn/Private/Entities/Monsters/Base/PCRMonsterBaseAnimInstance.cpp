// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

UPCRMonsterBaseAnimInstance::UPCRMonsterBaseAnimInstance(): bShouldMove(false), bCanAttack(true), bDead(false)
{
	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_MonsterDataAsset(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_MonsterDataAsset.Succeeded())
	{
		MonsterDataAsset = DA_MonsterDataAsset.Object;
	}
}

void UPCRMonsterBaseAnimInstance::HandleOwnerDead(APCRMonsterBaseCharacter* DeadMonster)
{
	StopAllMontages(0.1f);
	bDead = true;
}

void UPCRMonsterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedMonsterBaseCharacter = Cast<APCRMonsterBaseCharacter>(TryGetPawnOwner());
	if (CachedMonsterBaseCharacter)
	{
		CachedCharacterMovement = Cast<UCharacterMovementComponent>(CachedMonsterBaseCharacter->GetCharacterMovement());
		CachedMonsterBaseCharacter->OnDead.AddUObject(this, &UPCRMonsterBaseAnimInstance::HandleOwnerDead);
	}
}

void UPCRMonsterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 블루프린트 실행 시 방어 코드
	if (!CachedMonsterBaseCharacter)
	{
		return;
	}
	
	check(CachedCharacterMovement);
	Velocity = CachedCharacterMovement->Velocity;
	bShouldMove = Velocity.SizeSquared2D() >= FMath::Square(30);
}
