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
	FVector CameraOffset;

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
	float EricaNarrowShotFiringRate;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaWideShotFiringRate;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaRecallCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaNarrowShotForwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaNarrowShotBackwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaWideShotForwardDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaWideShotBackwardDamage;

public: // 에리카 대시
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMaxDashTime;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashInvincibleTime;

public: // 에리카 카드
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaWideShotAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardReturnSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaNarrowShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaWideShotRange;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaCardKnockBackPower;

public: // 세린
	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	float SerinMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	float SerinBasicChaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	float SerinChaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	float SerinRockSpeed;

public: // 몬스터 베이스
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	float DeadAfterDestroyTime;

	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	float AttackRadius;
	
public: // 토끼
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackPower;

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

	UPROPERTY(EditDefaultsOnly, Category = "EliteRabbit")
	float EliteRabbitMaxHpRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRabbit")
	float EliteRabbitMoveSpeedRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRabbit")
	float EliteRabbitAttackPowerRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRabbit")
	float EliteRabbitScaleRate;

public: // 근접 병사
	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float MeleeSoldierAttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "MeleeSoldier")
	float ShieldDestroyTimeAfterDrop;

	UPROPERTY(EditDefaultsOnly, Category = "EliteMeleeSoldier")
	float EliteMeleeSoldierMaxHpRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteMeleeSoldier")
	float EliteMeleeSoldierMoveSpeedRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteMeleeSoldier")
	float EliteMeleeSoldierAttackPowerRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteMeleeSoldier")
	float EliteMeleeSoldierScaleRate;

public: // 원거리 병사
	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float RangedSoldierAttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float SpearDestroyTimeAfterDrop;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float SpearSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "RangedSoldier")
	float SpearPredictionRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRangedSoldier")
	float EliteRangedSoldierMaxHpRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRangedSoldier")
	float EliteRangedSoldierMoveSpeedRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRangedSoldier")
	float EliteRangedSoldierAttackPowerRate;

	UPROPERTY(EditDefaultsOnly, Category = "EliteRangedSoldier")
	float EliteRangedSoldierScaleRate;

public: // 아이템
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float ItemSpawnRate;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float ItemSpawnRateByEliteMonster;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float ItemDestroyTime;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float MoreHpItemRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float ManyCardItemRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float HealItemRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float StrongAttackItemRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float LongerRangeItemRate;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float MaxHpIncreaseRate;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float HealRateByIncreaseMaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 MaxCardIncreaseCount;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float HealRate;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float DamageIncreaseValue;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float ShootRangeIncreaseDistance;
};
