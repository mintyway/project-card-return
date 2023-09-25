// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CParameterDataAsset.h"

UCParameterDataAsset::UCParameterDataAsset()
{
	CameraDistance = 1000.f;
	CameraLagSpeed = 10.f;
	CameraLagMaxDistance = 100.f;

	EricaMoveSpeed = 500.f;
	EricaAttackPower = 50.f;
	EricaRapidShotCoolTime = 0.3f;
	EricaBuckShotCoolTime = 1.f;
	EricaDashCoolTime = 1.f;
	EricaTotalDashTime = 0.25f;
	EricaDashDistance = 500.f;

	DeadAfterDestroyTime = 1.f;

	RabbitMaxHealthPoint = 100.f;
	RabbitAttackPower = 3.f;
	RabbitMoveSpeed = 300.f;
	RabbitAttackRange = 300.f;
	RabbitAttackSpeed = 1.f;

	ProjectilePoolSize = 100;

	EricaCardPoolSize = 30;

	ProjectileSpeed = 1000.f;

	EricaCardSpeed = 3000.f;
	EricaCardReturnSpeed = 6000.f;
	EricaCardRange = 1000.f;
	EricaCardReturnRange = 50.f;
}
