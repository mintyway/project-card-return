// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCRSpearActor.generated.h"

class UBoxComponent;
class UPCRMonsterDataAsset;

DECLARE_MULTICAST_DELEGATE(FDetachedSpearSignature);

UCLASS()
class PROJECTCARDRETURN_API APCRSpearActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRSpearActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	void DetachAndDelayedDestroy();
	
	FDetachedSpearSignature OnDetachedSpear;
	
private:
	void DelayedDestroy();
	void DestroyTimerCallback();

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;
};
