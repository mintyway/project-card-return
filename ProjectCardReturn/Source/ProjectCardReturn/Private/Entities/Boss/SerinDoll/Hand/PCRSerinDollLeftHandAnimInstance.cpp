// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollLeftHandAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"

UPCRSerinDollLeftHandAnimInstance::UPCRSerinDollLeftHandAnimInstance()
{
	// TODO: MaxScissorsAttackCount 파라미터화
}

void UPCRSerinDollLeftHandAnimInstance::PlayScissorsAttack()
{
	if (Montage_IsPlaying(nullptr))
	{
		return;
	}
	
	Montage_Play(SerinDollDataAsset->LeftHandScissorsAnimMontage);

	// FOnMontageEnded ScissorsAttack;
	// ScissorsAttack.BindUObject(this, &UPCRSerinDollLeftHandAnimInstance::ScissorsAttackCallback);
	// Montage_SetEndDelegate(ScissorsAttack, SerinDollDataAsset->LeftHandScissorsAnimMontage);
}

void UPCRSerinDollLeftHandAnimInstance::ScissorsAttackCallback(UAnimMontage* AnimMontage, bool bArg)
{
	if (OnScissorsAttackEnded.IsBound())
	{
		OnScissorsAttackEnded.Execute();

	}
}
