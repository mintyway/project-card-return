// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Character.h"
#include "PCRMonsterBaseCharacter.generated.h"

class UPCRParameterDataAsset;
class UProgressBar;
class UPCRUIDataAsset;
class UWidgetComponent;
class UPCRMonsterDataAsset;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMonsterBaseCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);

UCLASS()
class PROJECTCARDRETURN_API APCRMonsterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCRMonsterBaseCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Attack();

	void ChangeHP(float Amount);
	void Stun();

	FORCEINLINE const UPCRMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }
	FORCEINLINE const UPCRParameterDataAsset* GetParameterDataAsset() const { return ParameterDataAsset; }
	FORCEINLINE const UPCRUIDataAsset* GetUIDataAsset() const { return UIDataAsset; }
	FORCEINLINE bool IsAlive() const { return bIsAlive; }
	FORCEINLINE float GetAttackPower() const { return AttackPower; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetStunTime() const { return StunTime; }

	FOnHPChangeDelegate OnHPChange;
	FOnDeadDelegate OnDead;

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	virtual void HandleChangeHP();
	virtual void HandleDead();

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> HPBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UProgressBar> HPProgressBar;

	float HealthPoint;
	float MaxHealthPoint;
	uint32 bIsAlive : 1;

	float AttackPower;
	float MoveSpeed;
	float AttackRange;
	float AttackSpeed;
	float StunTime;

	void DestroyTimeCallback();
	
	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;
	
	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRUIDataAsset> UIDataAsset;

	float DeadAfterDestroyTime;
};
