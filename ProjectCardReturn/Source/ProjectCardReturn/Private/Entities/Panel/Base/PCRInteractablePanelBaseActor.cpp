// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Panel/Base/PCRInteractablePanelBaseActor.h"

#include "Entities/Panel/Base/PCRInteractablePanelDataAsset.h"
	
APCRInteractablePanelBaseActor::APCRInteractablePanelBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// static ConstructorHelpers::FObjectFinder<UPCRInteractablePanelDataAsset> DA_InteractablePanelDataAsset(TEXT("Refference"));
	
}

void APCRInteractablePanelBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRInteractablePanelBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

