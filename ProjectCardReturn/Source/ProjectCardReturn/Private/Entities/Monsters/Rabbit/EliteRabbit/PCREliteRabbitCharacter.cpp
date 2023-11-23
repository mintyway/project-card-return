// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/EliteRabbit/PCREliteRabbitCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteRabbitCharacter::APCREliteRabbitCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RabbitMaxHP * 2.0f;
		CurrentHP = MaxHP;
		AttackPower = ParameterDataAsset->RabbitAttackPower * 1.5f;
	}

	bIsElite = true;
	
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
}
