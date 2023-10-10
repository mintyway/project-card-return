// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRParameterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRParameterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPCRParameterDataAsset();

public:
	UPROPERTY(EditAnywhere, Category = "Game")
	bool bIsMonsterSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraPitch;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	double CameraOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraLagSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraLagMaxDistance;


	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaRapidShotCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaBuckShotCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMaxDashTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	int32 EricaCardCount;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardReturnSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardRapidShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardBuckShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardReleaseRange;


	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	float DeadAfterDestroyTime;


	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMaxHealthPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackSpeed;


	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMaxHealthPoint;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float ShieldDestroyTimeAfterDrop;
};
