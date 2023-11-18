// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "GameFramework/Actor.h"
#include "PCRShieldActor.generated.h"

class UPCRParameterDataAsset;
class UBoxComponent;
class UPCRMonsterDataAsset;
class APCREricaCardProjectile;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogShieldActor, Log, All);
DECLARE_MULTICAST_DELEGATE(FDetachedShieldSignature);
DECLARE_MULTICAST_DELEGATE(FDetachedCardSignature);

UCLASS()
class PROJECTCARDRETURN_API APCRShieldActor : public AActor, public IPCREricaCardInteractable
{
	GENERATED_BODY()

public:
	APCRShieldActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	virtual void BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard) override;
	
	void DetachAndDelayedDestroy();
	
	FDetachedShieldSignature OnDetachedShield;
	FDetachedCardSignature OnDetachedCard;

private:
	void DelayedDestroy();
	void HandleReturnCard(APCREricaCardProjectile* AttachedCard);
	void DestroyTimerCallback();

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	TMap<APCREricaCardProjectile*, FDelegateHandle> OnReturnCardBeginDelegateMap;
};
