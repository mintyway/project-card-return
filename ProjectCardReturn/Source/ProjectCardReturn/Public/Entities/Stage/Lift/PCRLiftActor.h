// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRLiftActor.generated.h"


class UPCRSoundPrimaryDataAsset;
DECLARE_LOG_CATEGORY_EXTERN(PCRLogRiftActor, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLiftUpDelegate);

namespace FMOD::Studio
{
	class EventInstance;
}

class APCREricaCharacter;
class UBoxComponent;
class UPCRStagePrimaryDataAsset;

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRLiftActor : public AActor
{
	GENERATED_BODY()

public:
	APCRLiftActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // 동작 섹션
	UFUNCTION()
	void LiftUp();

	void SerinPattern1Start();
	void SerinPattern1End();

	TArray<FVector> GetShuffleLocationForPattern1Target();

public: // Getter
	bool IsOverlappedPattern1() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Event", DisplayName = "리프트 상승 시작 시")
	FLiftUpDelegate OnLiftUpStart;

	UPROPERTY(BlueprintAssignable, Category = "Event", DisplayName = "리프트 상승 종료 시")
	FLiftUpDelegate OnLiftUpEnd;

private:
	enum class EState
	{
		LiftUp,
		SerinPattern1
	};

private: // 내부 함수 섹션
	void UpdateLiftUp(float DeltaTime);
	void UpdatePattern1OverlapCheck();


private: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRStagePrimaryDataAsset> StageDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;

private: // 컴포넌트 섹션
	UPROPERTY(VisibleAnywhere, Category = "Scene")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> LiftMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "ColliderComponents")
	TArray<TObjectPtr<UBoxComponent>> ColliderComponents;
	
	UPROPERTY(VisibleAnywhere, Category = "Event")
	TObjectPtr<UBoxComponent> Pattern1SuccessBoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Pattern1Targets")
	TArray<TObjectPtr<USceneComponent>> Pattern1TargetComponents;
	
	FMOD::Studio::EventInstance* LiftUpSoundInst;

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedErica;

private: // 데이터 섹션
	EState State;
	FVector StartLiftLocation;
	FVector EndLiftLocation;
	float MaxLiftHeight;
	float LiftingTime;
	float ElapsedTime;
	uint32 bIsOverlappedPattern1:1;
};
