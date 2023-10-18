// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRParameterDataAsset.h"

UPCRParameterDataAsset::UPCRParameterDataAsset()
{
	bIsMonsterSpawn = true;

	CameraFOV = 60.f;
	CameraDistance = 2000.f;
	CameraPitch = 40.f;
	CameraOffset = -125.0;
	CameraLagSpeed = 18.f;
	CameraLagMaxDistance = 100.f;

	EricaMoveSpeed = 750.f;
	BuckShotCount = 3;
	BuckShotAngle = 60.f;
	EricaNormalShotForwardDamage = 1.f;
	EricaNormalShotBackwardDamage = 1.5f;
	EricaBuckShotForwardDamage = 0.5f;
	EricaBuckShotBackwardDamage = 1.f;
	EricaNormalShotCooldownTime = 0.8f;
	EricaBuckShotCooldownTime = 0.6f;
	EricaDashCooldownTime = 1.f;
	EricaMaxDashTime = 0.25f;
	EricaDashDistance = 500.f;
	EricaCardCount = 9;
	EricaCardSpeed = 3000.f;
	EricaCardReturnSpeed = 6000.f;
	EricaCardNormalShotRange = 1300.f;
	EricaCardBuckShotRange = 700.f;
	EricaCardReleaseRange = 50.f;

	DeadAfterDestroyTime = 1.f;

	RabbitMaxHealthPoint = 5.f;
	RabbitAttackPower = 3.f;
	RabbitMoveSpeed = EricaMoveSpeed * 0.7f;
	RabbitAttackRange = 300.f;
	RabbitAttackSpeed = 1.f;

	MeleeSoldierMaxHealthPoint = 6.f;
	MeleeSoldierAttackPower = 34.f;
	MeleeSoldierMoveSpeed = EricaMoveSpeed * 0.5f;
	MeleeSoldierAttackRange = 300.f;
	MeleeSoldierAttackSpeed = 1.f;

	ShieldDestroyTimeAfterDrop = 1.f;
}
