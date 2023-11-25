// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/GameModeBase.h"
#include "PCRGameModeBase.generated.h"

class APCREricaCharacter;
class APCRSerinDollHeadCharacter;
class UPCRGameInstance;
class UFMODAudioComponent;
class APCRLiftActor;
class UPCRStagePrimaryDataAsset;
class UPCRParameterDataAsset;
class APCRMonsterGenerator;
class UPCRSoundPrimaryDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStage1EndSignature);

DECLARE_DELEGATE_TwoParams(FStage1MonsterCountSignature, int32, int32);

DECLARE_LOG_CATEGORY_EXTERN(PCRLogGameModeBase, Log, All);

UENUM()
enum class EStageState : uint8
{
	Stage = 1,
	SerinStage
};

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APCRGameModeBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public: // 동작
	UFUNCTION()
	void SpawnSerinDoll();
	
public: // Getter
	FORCEINLINE APCREricaCharacter* GetCachedEricaCharacter() const { return CachedEricaCharacter; }
	FORCEINLINE APCRLiftActor* GetLiftActor() const { return LiftActor; }
	FORCEINLINE int32 GetStage1MaxMonsterCount() { return Stage1TargetKillCount; }
	FORCEINLINE int32 GetStage1CurrentMonsterCount() { return Stage1TargetKillCount - Stage1TotalMonsterKillCount; }
	FORCEINLINE EStageState GetCurrentStageState() const { return CurrentStageState; }
	FORCEINLINE APCRSerinDollHeadCharacter* GetSerinDollHead() const { return SerinDollHead; }

	// 델리게이트
	FStage1EndSignature OnStage1End;
	FStage1MonsterCountSignature OnChangeStage1MonsterCount;

private: // 내부 함수 섹션
	void SpawnMonsterGenerators();
	void StartAllMonsterGenerators();
	void StopAllMonsterGeneratorsAndKillSpawnedMonsters();

	void HandleKillCount();
	void HandleSpawnMonster();

	void PlayStage1BGM();
	void PlayBossStageBGM();

	UFUNCTION()
	void LiftFloor();

private: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRStagePrimaryDataAsset> StageDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;

private: // 서브 액터 섹션
	UPROPERTY()
	TSubclassOf<APCRLiftActor> LiftActorClass;

	UPROPERTY(BlueprintReadOnly, Category = "Actor", meta = (AllowPrivateAccess = true))
	TObjectPtr<APCRLiftActor> LiftActor;

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<UPCRGameInstance> CachedPCRGameInstance;

	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter; 
	
	UPROPERTY()
	TObjectPtr<APCRSerinDollHeadCharacter> SerinDollHead;

private: // 데이터 섹션
	UPROPERTY()
	TArray<TObjectPtr<APCRMonsterGenerator>> MonsterGenerators;

	float GenerateInterval;
	int32 Phase;
	
	UPROPERTY(BlueprintReadWrite, Category = "Debug", meta = (allowPrivateAccess = true))
	int32 Stage1TotalMonsterKillCount;

	UPROPERTY(BlueprintReadWrite, Category = "Debug", meta = (allowPrivateAccess = true))
	int32 Stage1TargetKillCount;

	EStageState CurrentStageState;
};
