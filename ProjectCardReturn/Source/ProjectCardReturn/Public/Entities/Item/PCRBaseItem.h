// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRBaseItem.generated.h"

class UBoxComponent;

UENUM()
enum class EItemType : uint8
{
	SpeedUp,
	MoreCard,
	Heal,
	FastShoot,
	LongerRange
};

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

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;
};
