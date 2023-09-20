// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/EricaRoss/CEricaRossPlayerController.h"

#include "Entities/Players/EricaRoss/CEricaRossDataAsset.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Entities/Players/EricaRoss/CEricaRossCharacter.h"

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

	CachedEricaRossCharacter = Cast<ACEricaRossCharacter>(InPawn);
	RETURN_IF_INVALID(CachedEricaRossCharacter);
}

void ACEricaRossPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DataAsset->GetDefaultInputMappingContext(), 0);
	}
}

/**
 * 현재 마우스 커서의 위치를 반환합니다.\n
 * 실패 시 FVector::UnitX()를 반환합니다.
 * @return 현재 마우스 커서의 위치
 */
FVector ACEricaRossPlayerController::GetMouseDirection()
{
	FHitResult MouseClickedResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, MouseClickedResult))
	{
		FVector MouseClickedLocation = MouseClickedResult.Location;
		MouseClickedLocation.Z = 0.0;
		
		RETURN_IF_INVALID(CachedEricaRossCharacter, FVector::UnitX());
		FVector EricaRossLocation = CachedEricaRossCharacter->GetActorLocation();
		EricaRossLocation.Z = 0.0;
		
		FVector MouseDirection = (MouseClickedLocation - EricaRossLocation).GetSafeNormal();
		return MouseDirection;
	}
	
	return FVector::UnitX();
}

void ACEricaRossPlayerController::Shoot()
{
	GetMouseDirection();
	CachedEricaRossCharacter->Shoot();
}

void ACEricaRossPlayerController::Return()
{
	CachedEricaRossCharacter->Return();
}
