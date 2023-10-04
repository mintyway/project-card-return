// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRShieldActor.generated.h"

class UBoxComponent;
class UPCRMonsterDataAsset;
class APCREricaCardProjectile;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogShieldActor, Log, All);
DECLARE_MULTICAST_DELEGATE(FDetachedSignature);

UCLASS()
class PROJECTCARDRETURN_API APCRShieldActor : public AActor
{
	GENERATED_BODY()

public:
	APCRShieldActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FDetachedSignature OnDetachedCard;

private:
	UFUNCTION()
	void HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	void HandleReturnCard(APCREricaCardProjectile* AttachedCard);

	UPROPERTY(VisibleAnywhere, Category = "Box");
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

	TMap<APCREricaCardProjectile*, FDelegateHandle> OnReturnCardBeginDelegateMap;
};
