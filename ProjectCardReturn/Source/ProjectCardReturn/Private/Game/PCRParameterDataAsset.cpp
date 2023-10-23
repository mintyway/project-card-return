// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRParameterDataAsset.h"

UPCRParameterDataAsset::UPCRParameterDataAsset()
	: bIsMonsterSpawn(true), SpawnerGenerateInterval(10.f),

	  CameraFOV(60.f), CameraDistance(2000.f), CameraPitch(38.f), CameraOffset(-125.0), CameraLagSpeed(18.f), CameraLagMaxDistance(100.f),

	  EricaMaxHP(100.f), EricaMoveSpeed(750.f), EricaCardCount(9), EricaSingleShotFiringRate(1.25f), EricaMultiShotFiringRate(1.5f), EricaRecallCooldownTime(1.f), EricaSingleShotForwardDamage(1.f), EricaSingleShotBackwardDamage(2.5f), EricaMultiShotForwardDamage(0.5f), EricaMultiShotBackwardDamage(1.5f),

	  EricaDashCooldownTime(1.f), EricaDashDistance(500.f), EricaMaxDashTime(0.1f),

	  EricaMultiShotCount(3), EricaMultiShotAngle(30.f), EricaCardSpeed(5000.f), EricaCardReturnSpeed(6000.f), EricaSingleShotRange(1300.f), EricaMultiShotRange(700.f), EricaCardReleaseRange(50.f),

	  DeadAfterDestroyTime(1.f),

	  MeleeSoldierMaxHP(6.f), MeleeSoldierAttackPower(10.f), MeleeSoldierMoveSpeed(300.f), MeleeSoldierAttackRange(300.f), MeleeSoldierAttackRate(1.f), ShieldDestroyTimeAfterDrop(1.f),

	  RangedSoldierMaxHP(3.f), RangedSoldierAttackPower(5.f), RangedSoldierMoveSpeed(300.f), RangedSoldierAttackRange(1500.f), RangedSoldierAttackRate(1.f), SpearDestroyTimeAfterDrop(1.f), SpearSpeed(2000.f),

	  RabbitMaxHealthPoint(3.f), RabbitAttackPower(5.f), RabbitMoveSpeed(EricaMoveSpeed * 0.7f), RabbitAttackRange(300.f), RabbitAttackRate(1.f) {}
