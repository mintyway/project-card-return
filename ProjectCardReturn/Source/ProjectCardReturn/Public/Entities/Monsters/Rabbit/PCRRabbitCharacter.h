// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRRabbitCharacter.generated.h"

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
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void HandleDead() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	// TODO: 더미 용: 모델링 완료시 제거
	UPROPERTY(VisibleAnywhere, Category = "Dummy")
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;
	
	bool bCanAttack;
};
