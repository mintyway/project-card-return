// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRSerinDollPrimaryDataAsset.generated.h"

class UPCRSerinDollLeftHandAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SerinDoll", DisplayName = "세린 메시")
	TObjectPtr<USkeletalMesh> SerinDollMesh;

	UPROPERTY(EditAnywhere, Category = "SerinDollLeftHand", DisplayName = "세린 왼쪽 손")
	TObjectPtr<USkeletalMesh> SerinDollLeftHandMesh;

	UPROPERTY(EditAnywhere, Category = "SerinDollLeftHand", DisplayName = "세린 왼쪽 손 애니메이션 블루프린트")
	TSoftClassPtr<UPCRSerinDollLeftHandAnimInstance> SerinDollLeftAnimInstance;

	UPROPERTY(EditAnywhere, Category = "LeftHand", DisplayName = "왼쪽 손 가위 공격 애님 몽타주")
	TObjectPtr<UAnimMontage> LeftHandScissorsAnimMontage;

	UPROPERTY(EditAnywhere, Category = "SerinDollRightHands")
	TObjectPtr<USkeletalMesh> SerinDollRightHandMesh;
};
