// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRRabbitCharacter.generated.h"

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
	
private:
	UPROPERTY()
	TObjectPtr<UPCRRabbitAnimInstance> RabbitAnimInstance;
};
