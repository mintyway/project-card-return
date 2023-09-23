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
	FORCEINLINE UBehaviorTree* GetRabbitBehaviorTree() const { return RabbitBehaviorTree; }
	FORCEINLINE UBlackboardData* GetRabbitBlackBoard() const { return RabbitBlackBoard; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<USkeletalMesh> RabbitMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<UBehaviorTree> RabbitBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<UBlackboardData> RabbitBlackBoard;
};
