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

public: // 게임
	UPROPERTY(EditAnywhere, Category = "Game")
	uint32 bIsMonsterSpawn : 1;

	UPROPERTY(EditAnywhere, Category = "Game")
	float SpawnerGenerateInterval;

public: // 카메라
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

public: // 에리카 스탯
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	int32 EricaCardCount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaSingleShotFiringRate;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMultiShotFiringRate;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaRecallCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaSingleShotForwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaSingleShotBackwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMultiShotForwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMultiShotBackwardDamage;

public: // 에리카 대시
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMaxDashTime;

public: // 에리카 카드
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	int32 EricaMultiShotCount;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMultiShotAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardReturnSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaSingleShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMultiShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardReleaseRange;

public: // 몬스터 베이스
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	float DeadAfterDestroyTime;

public: // 근접 병사
	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float ShieldDestroyTimeAfterDrop;

public: // 원거리 병사
	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float SpearDestroyTimeAfterDrop;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float SpearSpeed;

public: // 토끼
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMaxHealthPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitGimmickMoveRange;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitJumpProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitWaitAndJumpProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveRightDiagonalProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveLeftDiagonalProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveDiagonalDistance;
};
