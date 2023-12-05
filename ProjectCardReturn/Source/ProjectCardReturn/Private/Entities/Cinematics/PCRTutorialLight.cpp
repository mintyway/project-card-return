// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Cinematics/PCRTutorialLight.h"

#include "FMODBlueprintStatics.h"
#include "Game/PCRSoundPrimaryDataAsset.h"
#include "Kismet/GameplayStatics.h"

APCRTutorialLight::APCRTutorialLight()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}
	
}

void APCRTutorialLight::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Light0"), Light0s);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Light1"), Light1s);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Light2"), Light2s);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Light3"), Light3s);

	SetLight1s(true);
	SetLight2s(true);
	SetLight3s(true);

	GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Cyan, TEXT("튜토리얼 라이트 생성"));
}

void APCRTutorialLight::TurnOnLights()
{
	const float LightDelayTime = 2.f;
	for (const auto& Light0 : Light0s)
	{
		Light0->Destroy();
	}
	
	UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->LightOn, true);
	
	FTimerHandle Light1TimerHandle;
	GetWorldTimerManager().SetTimer(Light1TimerHandle, FTimerDelegate::CreateUObject(this, &APCRTutorialLight::SetLight1s, false), LightDelayTime, false);
	FTimerHandle Light2TimerHandle;
	GetWorldTimerManager().SetTimer(Light2TimerHandle, FTimerDelegate::CreateUObject(this, &APCRTutorialLight::SetLight2s, false), LightDelayTime * 2, false);
	FTimerHandle Light3TimerHandle;
	GetWorldTimerManager().SetTimer(Light3TimerHandle, FTimerDelegate::CreateUObject(this, &APCRTutorialLight::SetLight3s, false), LightDelayTime * 3, false);

	GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Cyan, TEXT("튜토리얼 라이트 활성화"));
}

void APCRTutorialLight::SetLight1s(bool bIsHidden)
{
	for (const auto& Light1 : Light1s)
	{
		Light1->SetActorHiddenInGame(bIsHidden);
	}

	if (!bIsHidden)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->LightOn, true);
	}
}

void APCRTutorialLight::SetLight2s(bool bIsHidden)
{
	for (const auto& Light2 : Light2s)
	{
		Light2->SetActorHiddenInGame(bIsHidden);
	}

	if (!bIsHidden)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->LightOn, true);
	}
}

void APCRTutorialLight::SetLight3s(bool bIsHidden)
{
	for (const auto& Light3 : Light3s)
	{
		Light3->SetActorHiddenInGame(bIsHidden);
	}
	
	if (!bIsHidden)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->LightOn, true);
		OnLightEnd.Broadcast();
	}
}
