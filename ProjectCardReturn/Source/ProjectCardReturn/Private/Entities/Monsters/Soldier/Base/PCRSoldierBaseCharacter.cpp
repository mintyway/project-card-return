// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseAIController.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierCharacter);

APCRSoldierBaseCharacter::APCRSoldierBaseCharacter()
{
	AIControllerClass = APCRSoldierBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(50.f, 50.f);
	}

	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetupAttachment(GetCapsuleComponent());
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -86.5), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	if (GetCharacterMovement() && GetParameterDataAsset())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APCRSoldierBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APCRSoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSoldierBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
