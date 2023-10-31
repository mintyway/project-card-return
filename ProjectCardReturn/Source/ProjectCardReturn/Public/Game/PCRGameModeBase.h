// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/GameModeBase.h"
#include "PCRGameModeBase.generated.h"

class APCRLiftActor;
class UPCRStagePrimaryDataAsset;
class UPCRParameterDataAsset;
class APCRMonsterGenerator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStage1EndSignature);

DECLARE_LOG_CATEGORY_EXTERN(PCRLogGameModeBase, Log, All);

UENUM()
enum class EStageNumber : uint8
{
	Stage1 = 1,
	Stage2,
	StageBoss
};

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//pullrqtest
public:
	APCRGameModeBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private: // 내부 함수 섹션
	void SpawnMonsterGenerators();
	void StartAllMonsterGenerators();
	void StopAllMonsterGeneratorsAndKillSpawnedMonsters();
	
	void DisplayLogMonsterKillCount();
	void HandleKillCount();

	UFUNCTION()
	void LiftFloor();

	UFUNCTION()
	void SpawnSerinDoll();

public:
	FStage1EndSignature OnStage1End;
	
private: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRStagePrimaryDataAsset> StageDataAsset;
	
	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

private: // 서브 액터 섹션
	UPROPERTY()
	TSubclassOf<APCRLiftActor> LiftActorClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Actor", meta = (AllowPrivateAccess = true))
	TObjectPtr<APCRLiftActor> LiftActor;

private: // 데이터 섹션
	UPROPERTY()
	TArray<TObjectPtr<APCRMonsterGenerator>> MonsterGenerators;

	float GenerateInterval;
	int32 TotalMonsterKillCount;
	int32 Stage1TargetKillCount;

	EStageNumber CurrentStageNumber;
};
