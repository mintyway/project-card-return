// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/EricaRoss/CEricaRossPlayerController.h"

#include "Entities/Players/EricaRoss/CEricaRossDataAsset.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACEricaRossPlayerController::ACEricaRossPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UCEricaRossDataAsset> DA_EricaRoss(TEXT("/Script/ProjectCardReturn.CEricaRossDataAsset'/Game/DataAssets/DA_EricaRoss.DA_EricaRoss'"));
	if (DA_EricaRoss.Succeeded())
	{
		DataAsset = DA_EricaRoss.Object;
	}

	bShowMouseCursor = true;
}

void ACEricaRossPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		RETURN_IF_INVALID(DataAsset);
		EnhancedInputComponent->BindAction(DataAsset->GetShootInputAction(), ETriggerEvent::Started, this, &ACEricaRossPlayerController::Shoot);
		EnhancedInputComponent->BindAction(DataAsset->GetReturnInputAction(), ETriggerEvent::Started, this, &ACEricaRossPlayerController::Return);
	}
}

void ACEricaRossPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
}

void ACEricaRossPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DataAsset->GetDefaultInputMappingContext(), 0);
	}
}

void ACEricaRossPlayerController::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot!"));
}

void ACEricaRossPlayerController::Return()
{
	UE_LOG(LogTemp, Warning, TEXT("Return!"));
}
