// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRBaseItem.generated.h"

class APCREricaCharacter;
class UPCRParameterDataAsset;
class UPCRItemDataAsset;
class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class PROJECTCARDRETURN_API APCRBaseItem : public AActor
{
	GENERATED_BODY()
	
public:
	APCRBaseItem();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void DestroyTimerCallback();
	
	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;

protected:
	virtual void PlayerOverlapEvent(APCREricaCharacter* Player);

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRItemDataAsset> ItemDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
};
