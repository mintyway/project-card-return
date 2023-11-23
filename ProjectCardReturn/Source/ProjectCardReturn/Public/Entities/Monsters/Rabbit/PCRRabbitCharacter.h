// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRRabbitCharacter.generated.h"

UENUM()
enum class EDirection : uint8
{
	Left,
	Right
};

class UPCRRabbitAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRRabbitCharacter : public APCRMonsterBaseCharacter
{
	GENERATED_BODY()

public:
	APCRRabbitCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Attack() override;

public:
	virtual void Jump() override;
	
	FORCEINLINE float GetGimmickMoveRange() const { return GimmickMoveRange; }
	FORCEINLINE float GetJumpProbability() const { return JumpProbability; }
	FORCEINLINE float GetWaitAndJumpProbability() const { return WaitAndJumpProbability; }
	FORCEINLINE float GetMoveRightDiagonalProbability() const { return MoveRightDiagonalProbability; }
	FORCEINLINE float GetMoveLeftDiagonalProbability() const { return MoveLeftDiagonalProbability; }

	FVector GetRightDiagonal() const;
	FVector GetLeftDiagonal() const;
	
	void Hit();
	
private:
	float GimmickMoveRange;
	float JumpProbability;
	float WaitAndJumpProbability;
	float MoveRightDiagonalProbability;
	float MoveLeftDiagonalProbability;
	float MoveDiagonalDistance;

	UPROPERTY()
	TObjectPtr<UPCRRabbitAnimInstance> AnimInstance;
};
