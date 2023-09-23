// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "CMonsterBaseCharacter.generated.h"

class UProgressBar;
class UCUIDataAsset;
class UWidgetComponent;
class UCMonsterDataAsset;

DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);

UCLASS()
class PROJECTCARDRETURN_API ACMonsterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACMonsterBaseCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Attack();
	virtual float GetDetectRange() const { return DetectRange; };
	virtual float GetAttackRange() const { return AttackRange; };

	FORCEINLINE UCMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }
	FORCEINLINE UCUIDataAsset* GetUIDataAsset() const { return UIDataAsset; }

	FOnHPChangeDelegate OnHPChange;
	FOnDeadDelegate OnDead;

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void HandleHPChange();
	virtual void HandleDead();

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> HPBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UProgressBar> HPProgressBar;

	float MaxHealthPoint;
	float HealthPoint;
	float AttackPower;
	bool IsAlive;
	float MoveSpeed;
	float DetectRange;
	float AttackRange;

private:
	TObjectPtr<UCMonsterDataAsset> MonsterDataAsset;
	TObjectPtr<UCUIDataAsset> UIDataAsset;
};
