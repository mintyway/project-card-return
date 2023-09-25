// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "CParameterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UCParameterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UCParameterDataAsset();

public:
	FORCEINLINE float GetCameraDistance() const { return CameraDistance; }
	FORCEINLINE float GetCameraLagSpeed() const { return CameraLagSpeed; }
	FORCEINLINE float GetCameraLagMaxDistance() const { return CameraLagMaxDistance; }
	
	FORCEINLINE float GetEricaMoveSpeed() const { return EricaMoveSpeed; }
	FORCEINLINE float GetEricaAttackPower() const { return EricaAttackPower; }
	FORCEINLINE float GetEricaRapidShotCoolTime() const { return EricaRapidShotCoolTime; }
	FORCEINLINE float GetEricaBuckShotCoolTime() const { return EricaBuckShotCoolTime; }
	FORCEINLINE float GetEricaDashCoolTime() const { return EricaDashCoolTime; }
	FORCEINLINE float GetEricaTotalDashTime() const { return EricaTotalDashTime; }
	FORCEINLINE float GetEricaDashDistance() const { return EricaDashDistance; }
	
	FORCEINLINE float GetDeadAfterDestroyTime() const { return DeadAfterDestroyTime; }

	FORCEINLINE float GetRabbitMaxHealthPoint() const { return RabbitMaxHealthPoint; }
	FORCEINLINE float GetRabbitAttackPower() const { return RabbitAttackPower; }
	FORCEINLINE float GetRabbitMoveSpeed() const { return RabbitMoveSpeed; }
	FORCEINLINE float GetRabbitAttackSpeed() const { return RabbitAttackSpeed; }

	FORCEINLINE int32 GetProjectilePoolSize() const { return ProjectilePoolSize; }

	FORCEINLINE int32 GetEricaCardPoolSize() const { return EricaCardPoolSize; }

	FORCEINLINE float GetProjectileSpeed() const { return ProjectileSpeed; }

	FORCEINLINE float GetEricaCardSpeed() const { return EricaCardSpeed; }
	FORCEINLINE float GetEricaCardReturnSpeed() const { return EricaCardReturnSpeed; }
	FORCEINLINE float GetEricaCardRange() const { return EricaCardRange; }
	FORCEINLINE float GetEricaCardReturnRange() const { return EricaCardReturnRange; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float CameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float CameraLagSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float CameraLagMaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaAttackPower;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaRapidShotCoolTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaBuckShotCoolTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaTotalDashTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	float EricaDashCoolTime;

	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	float DeadAfterDestroyTime;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMaxHealthPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	float RabbitAttackSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectilePool")
	int32 ProjectilePoolSize;

	UPROPERTY(EditDefaultsOnly, Category = "EricaCardPool")
	int32 EricaCardPoolSize;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "EricaCard")
	float EricaCardSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "EricaCard")
	float EricaCardReturnSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "EricaCard")
	float EricaCardRange;

	UPROPERTY(EditDefaultsOnly, Category = "EricaCard")
	float EricaCardReturnRange;
};
