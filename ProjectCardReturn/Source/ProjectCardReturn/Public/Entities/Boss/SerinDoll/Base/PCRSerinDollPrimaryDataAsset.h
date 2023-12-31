// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRSerinDollPrimaryDataAsset.generated.h"

class UPCRSerinDollHeadAnimInstance;
class UNiagaraSystem;
class UPCRSerinDollHandAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> HeadMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> HandMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMesh> Pattern1ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSoftClassPtr<UPCRSerinDollHeadAnimInstance> HeadAnimInstanceClass;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSoftClassPtr<UPCRSerinDollHandAnimInstance> HandAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HeadStartAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HeadHitAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HandStartAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> RockAttackAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> PaperAttackAnimMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ScissorsAttackAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HeadPattern1AnimMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HandPattern1AnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HeadPattern2AnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HandPattern2AnimMontage;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> RockAttackEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> RockAttackHitEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> PaperAttackEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> PaperAttackHitEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> ScissorsAttackEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> ScissorsAttackHitEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> Pattern1ThrowEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> Pattern1BombEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> Pattern1BombTimerEffect;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> SparkEffect;
};
