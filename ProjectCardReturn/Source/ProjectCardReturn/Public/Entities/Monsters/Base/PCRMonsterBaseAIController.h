// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "AIController.h"
#include "PCRMonsterBaseAIController.generated.h"

class APCRMonsterBaseCharacter;
class UPCRMonsterDataAsset;
class UPCRMonsterBaseAnimInstance;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMonsterBaseAIController, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRMonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	APCRMonsterBaseAIController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	void ApplyStun(float StunTime);
	
	FORCEINLINE const UPCRMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }

	static const FName TargetKey;
	static const FName IsDetectedKey;

protected:
	void SetTarget();

protected: // 데이터 에셋
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

private:
	void StunTimerCallback();

private: // 캐시
	TObjectPtr<APCRMonsterBaseCharacter> CachedMonsterCharacter;

private: // 데이터
	FTimerHandle StunTimerHandle;
	uint32 bIsStunned:1;
};
