// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "UObject/Interface.h"
#include "PCREricaCardInteractable.generated.h"

class APCREricaCardProjectile;

UINTERFACE(MinimalAPI)
class UPCREricaCardInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTCARDRETURN_API IPCREricaCardInteractable
{
	GENERATED_BODY()

public:
	virtual void BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard) = 0;
};
