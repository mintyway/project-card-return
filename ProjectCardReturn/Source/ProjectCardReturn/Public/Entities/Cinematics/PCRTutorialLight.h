// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRTutorialLight.generated.h"

class UPCRSoundPrimaryDataAsset;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLightSignature);

UCLASS()
class PROJECTCARDRETURN_API APCRTutorialLight : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRTutorialLight();

protected:
	virtual void BeginPlay() override;

public: // 동작
	UFUNCTION(BlueprintCallable)
	void TurnOnLights();

public: // 델리게이트
	UPROPERTY(BlueprintAssignable)
	FLightSignature OnLightEnd;
	
private:
	void SetLight1s(bool bIsHidden);
	void SetLight2s(bool bIsHidden);
	void SetLight3s(bool bIsHidden);

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<UPCRSoundPrimaryDataAsset> SoundDataAsset;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Light0s;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Light1s;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Light2s;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Light3s;
};
