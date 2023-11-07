// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRBaseItem.generated.h"

class UPCRParameterDataAsset;
class UPCRItemDataAsset;
class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class PROJECTCARDRETURN_API APCRBaseItem : public AActor, public IPCREricaCardInteractable
{
	GENERATED_BODY()
	
public:
	APCRBaseItem();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void HandleReturnCard(APCREricaCardProjectile* AttachedCard);
	void DestroyTimerCallback();
	
	UFUNCTION()
	void HandleItemHit(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;

protected:
	virtual void BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) override;
	virtual void PlayerHitEvent(APCREricaCharacter* Player);

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRItemDataAsset> ItemDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	uint32 bInteractCard;
};
