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

	FORCEINLINE const UPCRMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }
	FORCEINLINE const UPCRParameterDataAsset* GetParameterDataAsset() const { return ParameterDataAsset; }
	FORCEINLINE const UPCRUIDataAsset* GetUIDataAsset() const { return UIDataAsset; }
	FORCEINLINE bool IsAlive() const { return bIsAlive; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }

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
	bool bIsAlive;
	float MoveSpeed;
	float AttackRange;
	float AttackSpeed;
	
private:
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
	TObjectPtr<const UPCRUIDataAsset> UIDataAsset;

	float DeadAfterDestroyTime;
};
