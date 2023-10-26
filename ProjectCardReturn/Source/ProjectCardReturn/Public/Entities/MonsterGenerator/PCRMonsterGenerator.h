// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRMonsterGenerator.generated.h"

class UPCRParameterDataAsset;
class APCRMonsterBaseCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMonsterGenerator, Log, All);

DECLARE_DELEGATE(FSpawnedMonsterDeadSignature);

// TODO: 몬스터 이벤트 추적, 몬스터 개수 파악, 몬스터 스폰 개수 제한
UCLASS()
class PROJECTCARDRETURN_API APCRMonsterGenerator : public AActor
{
	GENERATED_BODY()

public:
	APCRMonsterGenerator();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public: // 동작 섹션
	void Start(UClass* MonsterClass, float Interval);
	void Stop();
	void KillSpawnedMonsters();

public: // 델리게이트 섹션
	FSpawnedMonsterDeadSignature OnSpawnedMonsterDead;

public: // Getter, Setter 섹션
	FORCEINLINE int32 GetMonsterKillCount() const { return MonsterKillCount; }

private: // 내부 함수 섹션
	void SpawnMonster(UClass* MonsterClass);

	void RemoveDeadMonster(APCRMonsterBaseCharacter* ApcrMonsterBaseCharacter);

private: // 데이터 에셋
	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
	
private: // 컴포넌트 섹션
	UPROPERTY(VisibleAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> RootSceneComponent;

private: // 스폰 데이터 섹션
	FTimerHandle SpawnTimerHandle;
	float SpawnRangeRadius;

private: // 몬스터 관리 섹션
	UPROPERTY()
	TArray<TObjectPtr<APCRMonsterBaseCharacter>> SpawnMonsters;

	int32 MonsterKillCount;
};
