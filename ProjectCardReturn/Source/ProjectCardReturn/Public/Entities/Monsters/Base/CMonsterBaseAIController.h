// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "AIController.h"
#include "CMonsterBaseAIController.generated.h"

class UCMonsterDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACMonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACMonsterBaseAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	FORCEINLINE const UCMonsterDataAsset* GetMonsterDataAsset() const { return MonsterDataAsset; }

	static const FName TargetKey;

private:
	void SetTarget();
	TObjectPtr<const UCMonsterDataAsset> MonsterDataAsset;
};
