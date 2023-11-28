// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRParameterDataAsset.h"

UPCRParameterDataAsset::UPCRParameterDataAsset() :
	// 게임
	bIsMonsterSpawn(true),
	SpawnerGenerateInterval(10.f),

	// 카메라
	CameraFOV(40.f),
	CameraDistance(3500.f),
	CameraPitch(35.f),
	CameraOffset(0.0, 0.0, 0.0),
	CameraLagSpeed(18.f),
	CameraLagMaxDistance(100.f),

	// 에리카 스탯
	EricaMaxHP(100.f),
	EricaMoveSpeed(750.f),
	EricaCardCount(15),
	EricaNarrowShotFiringRate(5.f),
	EricaWideShotFiringRate(5.f),
	EricaRecallCooldownTime(1.f),
	EricaNarrowShotForwardDamage(1.f),
	EricaNarrowShotBackwardDamage(2.f),
	EricaWideShotForwardDamage(1.5f),
	EricaWideShotBackwardDamage(3.f),

	// 에리카 대시
	EricaDashCooldownTime(1.f),
	EricaDashDistance(750.f),
	EricaMaxDashTime(0.3f),
	EricaDashInvincibleTime(0.2f),

	// 에리카 카드
	EricaWideShotAngle(25.f),
	EricaCardSpeed(10000.f),
	EricaCardReturnSpeed(10000.f),
	EricaNarrowShotRange(1300.f),
	EricaWideShotRange(800.f),
	EricaCardKnockBackPower(1500.f),

	// 세린
	SerinMoveSpeed(500.f),
	SerinBasicChaseSpeed(500.f),
	SerinChaseSpeed(1000.f),
	SerinRockSpeed(3000.f),
	SerinSpawnLocationOffset(FVector(1200.0f, 0.0f, -250.0f)),

	// 몬스터
	MonsterDestroyTimeAfterDead(2.f),
	MonsterAttackRadius(50.0f),
	MonsterGeneratorSpawnRangeRadius(100.0f),
	MonsterGeneratorLocation1(FVector(-2300.0f, -2200.0f, 100.0f)),
	MonsterGeneratorLocation2(FVector(1700.0f, -2300.0f, 100.0f)),
	MonsterGeneratorLocation3(FVector(1700.0f, 2100.0f, 100.0f)),
	MonsterGeneratorLocation4(FVector(-2300.0f, 2200.0f, 100.0f)),
	MonsterSpawnTimeInPhase1(7.0f),
	MonsterSpawnTimeInPhase2(6.0f),
	MonsterSpawnTimeInPhase3(5.0f),
	MonsterTargetKillCountInPhase1(40),
	MonsterTargetKillCountInPhase2(80),
	MonsterTargetKillCountInPhase3(100),

	// 엘리트 몬스터
	EliteMonsterSpawnPerCountInPhase1(15),
	EliteMonsterSpawnPerCountInPhase2(15),
	EliteMonsterSpawnPerCountInPhase3(10),

	// 토끼
	RabbitMaxHP(3.f),
	RabbitMoveSpeed(EricaMoveSpeed * 0.7f),
	RabbitAttackPower(5.f),
	RabbitAttackRange(300.f),
	RabbitAttackRate(1.f),
	RabbitGimmickMoveRange(1000.f),
	RabbitJumpProbability(0.2f),
	RabbitWaitAndJumpProbability(0.25f),
	RabbitMoveRightDiagonalProbability(0.33f),
	RabbitMoveLeftDiagonalProbability(0.5f),
	RabbitMoveDiagonalDistance(300.f),
	RabbitMinSpawnCountByOnceInPhase1(2),
	RabbitMaxSpawnCountByOnceInPhase1(3),
	RabbitMinSpawnCountByOnceInPhase2(3),
	RabbitMaxSpawnCountByOnceInPhase2(5),
	RabbitMinSpawnCountByOnceInPhase3(3),
	RabbitMaxSpawnCountByOnceInPhase3(7),

	// 엘리트 토끼
	EliteRabbitSpawnRate(0.4f),
	EliteRabbitMaxHpRate(2.0f),
	EliteRabbitMoveSpeedRate(1.0f),
	EliteRabbitAttackPowerRate(1.5f),
	EliteRabbitScaleRate(2.0f),

	// 근접 병사
	MeleeSoldierMaxHP(6.f),
	MeleeSoldierMoveSpeed(300.f),
	MeleeSoldierAttackPower(10.f),
	MeleeSoldierAttackRange(300.f),
	MeleeSoldierAttackRate(1.f),
	ShieldDestroyTimeAfterDrop(1.f),
	MeleeSoldierMinSpawnCountByOnceInPhase1(1),
	MeleeSoldierMaxSpawnCountByOnceInPhase1(3),
	MeleeSoldierMinSpawnCountByOnceInPhase2(2),
	MeleeSoldierMaxSpawnCountByOnceInPhase2(4),
	MeleeSoldierMinSpawnCountByOnceInPhase3(3),
	MeleeSoldierMaxSpawnCountByOnceInPhase3(7),

	// 엘리트 근접 병사
	EliteMeleeSoldierSpawnRate(0.4f),
	EliteMeleeSoldierMaxHpRate(2.0f),
	EliteMeleeSoldierMoveSpeedRate(1.0f),
	EliteMeleeSoldierAttackPowerRate(1.5f),
	EliteMeleeSoldierScaleRate(1.5f),

	// 원거리 병사
	RangedSoldierMaxHP(3.f),
	RangedSoldierMoveSpeed(300.f),
	RangedSoldierAttackPower(5.f),
	RangedSoldierAttackRange(1500.f),
	RangedSoldierAttackRate(1.f),
	SpearDestroyTimeAfterDrop(3.f),
	SpearSpeed(1500.f),
	SpearPredictionRate(0.5f),
	RangedSoldierMinSpawnCountByOnceInPhase1(0),
	RangedSoldierMaxSpawnCountByOnceInPhase1(1),
	RangedSoldierMinSpawnCountByOnceInPhase2(0),
	RangedSoldierMaxSpawnCountByOnceInPhase2(3),
	RangedSoldierMinSpawnCountByOnceInPhase3(2),
	RangedSoldierMaxSpawnCountByOnceInPhase3(5),

	// 엘리트 원거리 병사
	EliteRangedSoldierSpawnRate(0.2f),
	EliteRangedSoldierMaxHpRate(1.5f),
	EliteRangedSoldierMoveSpeedRate(1.3f),
	EliteRangedSoldierAttackPowerRate(1.5f),
	EliteRangedSoldierScaleRate(1.5f),

	// 아이템 스폰
	ItemSpawnRate(0.4f),
	ItemSpawnRateByEliteMonster(1.0f),
	ItemDestroyTime(10.0f),
	MoreHpItemRate(0.0f),
	ManyCardItemRate(0.0f),
	HealItemRate(1.0f),
	StrongAttackItemRate(0.0f),
	LongerRangeItemRate(0.0f),
	HealItemSpawnLocationByGame1(FVector(1500.0f, 0.0f, 100.0f)),
	HealItemSpawnLocationByGame2(FVector(-1500.0f, -2000.0f, 100.0f)),
	HealItemSpawnLocationByGame3(FVector(-1500.0f, 2000.0f, 100.0f)),
	HealItemSpawnPerCountByGame(20),

	// 아이템 스탯
	MaxHpIncreaseRate(0.05f),
	HealRateByIncreaseMaxHP(0.05f),
	MaxCardIncreaseCount(3),
	HealRate(0.1f),
	DamageIncreaseValue(0.5f),
	ShootRangeIncreaseDistance(100)
{
}
