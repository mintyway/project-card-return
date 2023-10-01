// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRMonsterBaseGenerator.generated.h"

class APCRMonsterBaseCharacter;

// TODO: 몬스터 이벤트 추적, 몬스터 개수 파악, 몬스터 스폰 개수 제한
UCLASS()
class PROJECTCARDRETURN_API APCRMonsterBaseGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRMonsterBaseGenerator();

protected:
	virtual void BeginPlay() override;

public:
	void Start(UClass* MonsterClass, float Interval);
	void Stop();

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	void SpawnMonster(UClass* MonsterClass) const;
	
	TObjectPtr<APCRMonsterBaseCharacter> CurrentGenerateMonster;
	FTimerHandle SpawnTimerHandle;
	float GenerateInterval;
	float SpawnRangeRadius;
};
