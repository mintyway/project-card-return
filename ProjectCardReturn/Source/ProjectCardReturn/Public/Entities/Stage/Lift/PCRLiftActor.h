// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRLiftActor.generated.h"

class UPCRStagePrimaryDataAsset;

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRLiftActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRLiftActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // 동작 섹션
	void LiftUp();

private: // 내부 함수 섹션
	void HandleLeftUp(float DeltaTime);
	
private: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCRStagePrimaryDataAsset> StageDataAsset;
	
private: // 컴포넌트 섹션
	UPROPERTY(VisibleAnywhere, Category = "Scene")
	TObjectPtr<USceneComponent> SceneComponent; 
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> LiftMeshComponent;

private: // 데이터 섹션
	FVector StartLiftLocation;
	FVector EndLiftLocation;
	float MaxLiftHeight;
	float LiftingTime;
	float ElapsedTime;

};
