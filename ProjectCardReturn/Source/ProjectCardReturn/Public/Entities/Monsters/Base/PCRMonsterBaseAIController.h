// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "AIController.h"
#include "PCRMonsterBaseAIController.generated.h"

class UPCRMonsterDataAsset;
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
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	FORCEINLINE const UPCRMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }

	static const FName TargetKey;

private:
	void SetTarget();
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;
};
