// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

class ACEricaCardProjectilePool;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
