// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class ACMonsterBaseGenerator;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	TObjectPtr<ACMonsterBaseGenerator> MonsterGenerator1;
	TObjectPtr<ACMonsterBaseGenerator> MonsterGenerator2;
	TObjectPtr<ACMonsterBaseGenerator> MonsterGenerator3;
	TObjectPtr<ACMonsterBaseGenerator> MonsterGenerator4;
};
