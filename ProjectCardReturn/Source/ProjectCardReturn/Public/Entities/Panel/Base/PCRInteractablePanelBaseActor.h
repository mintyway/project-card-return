// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRInteractablePanelBaseActor.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRInteractablePanelBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRInteractablePanelBaseActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

};
