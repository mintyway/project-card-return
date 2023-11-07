// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRBaseItem.generated.h"

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

public:
	virtual void BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) override;

private:
	void HandleReturnCard(APCREricaCardProjectile* AttachedCard);
	void HandleItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void DestroyTimerCallback();

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRItemDataAsset> ItemDataAsset;
};
