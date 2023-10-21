// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRParameterDataAsset.h"

UPCRParameterDataAsset::UPCRParameterDataAsset()
{
	bIsMonsterSpawn = true;

	CameraFOV = 60.f;
	CameraDistance = 2000.f;
	CameraPitch = 38.f;
	CameraOffset = -125.0;
	CameraLagSpeed = 18.f;
	CameraLagMaxDistance = 100.f;

	EricaMoveSpeed = 750.f;
	BuckShotCount = 3;
	BuckShotAngle = 30.f;
	EricaNormalShotForwardDamage = 1.f;
	EricaNormalShotBackwardDamage = 2.5f;
	EricaBuckShotForwardDamage = 0.5f;
	EricaBuckShotBackwardDamage = 1.5f;
	EricaNormalShotCooldownTime = 0.8f;
	EricaBuckShotCooldownTime = 0.6f;
	EricaReturnCardCooldownTime = 1.f;
	EricaDashCooldownTime = 1.f;
	EricaMaxDashTime = 0.1f;
	EricaDashDistance = 500.f;
	EricaCardCount = 9;
	EricaCardSpeed = 5000.f;
	EricaCardReturnSpeed = 6000.f;
	EricaCardNormalShotRange = 1300.f;
	EricaCardBuckShotRange = 700.f;
	EricaCardReleaseRange = 50.f;

	DeadAfterDestroyTime = 1.f;

	RabbitMaxHealthPoint = 3.f;
	RabbitAttackPower = 3.f;
	RabbitMoveSpeed = EricaMoveSpeed * 0.7f;
	RabbitAttackRange = 300.f;
	RabbitAttackSpeed = 1.f;

	MeleeSoldierMaxHealthPoint = 6.f;
	MeleeSoldierAttackPower = 10.f;
	MeleeSoldierMoveSpeed = EricaMoveSpeed * 0.5f;
	MeleeSoldierAttackRange = 300.f;
	MeleeSoldierAttackSpeed = 1.f;
	ShieldDestroyTimeAfterDrop = 3.f;
	
	RangedSoldierMaxHealthPoint = 6.f;
	RangedSoldierAttackPower = 10.f;
	RangedSoldierMoveSpeed = 300.f;
	RangedSoldierAttackRange = 1500.f;
	RangedSoldierAttackSpeed = 1.f;
	SpearDestroyTimeAfterDrop = 3.f;
	SpearSpeed = 2000.f;
}
