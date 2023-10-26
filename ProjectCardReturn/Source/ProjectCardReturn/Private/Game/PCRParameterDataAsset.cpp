// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRParameterDataAsset.h"

UPCRParameterDataAsset::UPCRParameterDataAsset()
	: bIsMonsterSpawn(true), SpawnerGenerateInterval(10.f),

	  CameraFOV(30.f), CameraDistance(3500.f), CameraPitch(25.f), CameraOffset(0.0, 0.0, 0.0), CameraLagSpeed(18.f), CameraLagMaxDistance(100.f),

	  EricaMaxHP(100.f), EricaMoveSpeed(750.f), EricaCardCount(15), EricaNarrowShotFiringRate(2.f), EricaWideShotFiringRate(2.f), EricaRecallCooldownTime(1.f), EricaNarrowShotForwardDamage(1.f), EricaNarrowShotBackwardDamage(2.5f), EricaWideShotForwardDamage(0.5f), EricaWideShotBackwardDamage(1.5f),

	  EricaDashCooldownTime(1.f), EricaDashDistance(500.f), EricaMaxDashTime(0.1f), EricaWideShotAngle(30.f), EricaCardSpeed(5000.f), EricaCardReturnSpeed(6000.f), EricaNarrowShotRange(1300.f), EricaWideShotRange(700.f), EricaCardReleaseRange(50.f),

	  DeadAfterDestroyTime(1.f), HitStopTime(0.1f),

	  MeleeSoldierMaxHP(6.f), MeleeSoldierAttackPower(10.f), MeleeSoldierMoveSpeed(300.f), MeleeSoldierAttackRange(300.f), MeleeSoldierAttackRate(1.f), ShieldDestroyTimeAfterDrop(1.f),

	  RangedSoldierMaxHP(3.f), RangedSoldierAttackPower(5.f), RangedSoldierMoveSpeed(300.f), RangedSoldierAttackRange(1500.f), RangedSoldierAttackRate(1.f), SpearDestroyTimeAfterDrop(1.f), SpearSpeed(2000.f),

	  RabbitMaxHealthPoint(3.f), RabbitAttackPower(5.f), RabbitMoveSpeed(EricaMoveSpeed * 0.7f), RabbitAttackRange(300.f), RabbitAttackRate(1.f) {}
