// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "ShieldActor.generated.h"

class UPCRMonsterDataAsset;
class APCRBaseProjectile;
UCLASS()
class PROJECTCARDRETURN_API AShieldActor : public AActor
{
	GENERATED_BODY()

public:
	AShieldActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

	TMap<APCRBaseProjectile*, FDelegateHandle> OnReleaseProjectileDelegateMap;
};
