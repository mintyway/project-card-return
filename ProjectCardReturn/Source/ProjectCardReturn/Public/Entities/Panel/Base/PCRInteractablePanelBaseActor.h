// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "GameFramework/Actor.h"
#include "PCRInteractablePanelBaseActor.generated.h"

class UPCRInteractablePanelDataAsset;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogInteractablePanelBaseActor, Log, All);

UENUM()
enum class EState : uint8
{
	Standing = 1,
	Tipping,
	Tipped,
	StandingUp,
	Invalid = 0
};

UCLASS()
class PROJECTCARDRETURN_API APCRInteractablePanelBaseActor : public AActor, public IPCREricaCardInteractable
{
	GENERATED_BODY()

public:
	APCRInteractablePanelBaseActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) override;

	FORCEINLINE const UPCRInteractablePanelDataAsset* GetMonsterDataAsset() { return MonsterDataAsset; }

private:
	UFUNCTION()
	void HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void HandleTipping(float DeltaTime);
	void StandUpCooldownTimerCallback();
	void HandleStandingUp(float DeltaTime);
	void HandleReturnCard(APCREricaCardProjectile* AttachedCard);
	void EnableCollisionDetection();
	void DisableCollisionDetection();
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TObjectPtr<const UPCRInteractablePanelDataAsset> MonsterDataAsset;

	TMap<APCREricaCardProjectile*, FDelegateHandle> OnReturnCardBeginDelegateMap;
	EState CurrentState;
	FRotator StartRotation;
	FRotator EndRotation;
	float TippingTime;
	float ElapsedTime;
	float StandUpCooldownTime;
};
