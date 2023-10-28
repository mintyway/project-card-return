// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRMonsterDataAsset.generated.h"

class UBehaviorTree;
class UBlackboardData;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRMonsterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Soldier")
	TObjectPtr<UBehaviorTree> SoldierBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Soldier")
	TObjectPtr<UBlackboardData> SoldierBlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	TObjectPtr<USkeletalMesh> MeleeSoldierMesh;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	TObjectPtr<UStaticMesh> ShieldMesh;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	TSoftClassPtr<UAnimInstance> MeleeSoldierAnimationBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	TObjectPtr<UAnimMontage> MeleeSoldierShieldAttackAnimationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	TObjectPtr<UAnimMontage> MeleeSoldierSpearAttackAnimationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	TObjectPtr<USkeletalMesh> RangedSoldierMesh;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	TObjectPtr<UStaticMesh> SpearMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	TSoftClassPtr<UAnimInstance> RangedSoldierAnimationBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	TObjectPtr<UAnimMontage> RangedSoldierThrowAnimationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<UBehaviorTree> RabbitBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<UBlackboardData> RabbitBlackBoard;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<USkeletalMesh> RabbitMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TSoftClassPtr<UAnimInstance> RabbitAnimationBlueprint;
};
