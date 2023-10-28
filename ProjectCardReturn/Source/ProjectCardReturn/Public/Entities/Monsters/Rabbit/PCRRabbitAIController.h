// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "PCRRabbitAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRRabbitAIController : public APCRMonsterBaseAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	static const FName IsGimmickMoveKey;
	static const FName IsJumpKey;
	static const FName IsWaitAndJumpKey;
	static const FName IsMoveRightDiagonalKey;
	static const FName IsMoveLeftDiagonalKey;
	static const FName RightDiagonalKey;
	static const FName LeftDiagonalKey;
};
