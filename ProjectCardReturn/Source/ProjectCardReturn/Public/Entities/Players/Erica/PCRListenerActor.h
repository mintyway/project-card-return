// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRListenerActor.generated.h"

UCLASS()
class PROJECTCARDRETURN_API APCRListenerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRListenerActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Init(AActor* NewActor);
	FORCEINLINE USceneComponent* GetSceneComponent() const { return SceneComponent; }

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY()
	TObjectPtr<AActor> Target;

};
