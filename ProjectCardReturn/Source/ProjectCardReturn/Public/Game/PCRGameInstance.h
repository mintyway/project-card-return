// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/GameInstance.h"
#include "PCRGameInstance.generated.h"

class APCREricaCardProjectilePool;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
