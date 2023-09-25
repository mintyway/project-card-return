// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "CMonsterDataAsset.generated.h"

class UBehaviorTree;
class UBlackboardData;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UCMonsterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE USkeletalMesh* GetRabbitMesh() const { return RabbitMesh; }
	FORCEINLINE UBehaviorTree* GetDefaultBehaviorTree() const { return DefaultBehaviorTree; }
	FORCEINLINE UBlackboardData* GetDefaultBlackBoard() const { return DefaultBlackBoard; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	TObjectPtr<UBlackboardData> DefaultBlackBoard;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<USkeletalMesh> RabbitMesh;
};
