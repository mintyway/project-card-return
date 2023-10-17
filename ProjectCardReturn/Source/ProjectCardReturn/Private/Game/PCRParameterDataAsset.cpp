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
	EricaAttackPower = 1.f;
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

	RabbitMaxHealthPoint = 3.f;
	RabbitAttackPower = 3.f;
	RabbitMoveSpeed = 300.f;
	RabbitAttackRange = 300.f;
	RabbitAttackSpeed = 1.f;

	MeleeSoldierMaxHealthPoint = 1.f;
	MeleeSoldierAttackPower = 34.f;
	MeleeSoldierMoveSpeed = 300.f;
	MeleeSoldierAttackRange = 300.f;
	MeleeSoldierAttackSpeed = 1.f;

	ShieldDestroyTimeAfterDrop = 1.f;
}
