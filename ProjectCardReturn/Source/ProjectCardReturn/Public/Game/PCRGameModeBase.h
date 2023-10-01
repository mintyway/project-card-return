// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/GameModeBase.h"
#include "PCRGameModeBase.generated.h"

class APCRMonsterBaseGenerator;
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
	virtual void BeginPlay() override;

public:
	TObjectPtr<APCRMonsterBaseGenerator> MonsterGenerator1;
	TObjectPtr<APCRMonsterBaseGenerator> MonsterGenerator2;
	TObjectPtr<APCRMonsterBaseGenerator> MonsterGenerator3;
	TObjectPtr<APCRMonsterBaseGenerator> MonsterGenerator4;
};
