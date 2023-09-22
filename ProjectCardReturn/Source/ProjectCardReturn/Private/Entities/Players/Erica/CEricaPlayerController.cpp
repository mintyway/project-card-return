// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/CEricaPlayerController.h"

#include "Entities/Players/Erica/CEricaDataAsset.h"
#include "Entities/Players/Erica/CEricaCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACEricaPlayerController::ACEricaPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UCEricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.CEricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		DataAsset = DA_Erica.Object;
	}

	bShowMouseCursor = true;
}

void ACEricaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		RETURN_IF_INVALID(DataAsset);
		EnhancedInputComponent->BindAction(DataAsset->GetShootInputAction(), ETriggerEvent::Triggered, this, &ACEricaPlayerController::Shoot);
		EnhancedInputComponent->BindAction(DataAsset->GetReturnInputAction(), ETriggerEvent::Triggered, this, &ACEricaPlayerController::Return);
	}
}

void ACEricaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedEricaCharacter = Cast<ACEricaCharacter>(InPawn);
	RETURN_IF_INVALID(CachedEricaCharacter);
}

void ACEricaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		RETURN_IF_INVALID(IsValid(DataAsset));
		Subsystem->AddMappingContext(DataAsset->GetDefaultInputMappingContext(), 0);
	}
}

void ACEricaPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator MouseDirectionRotator = FRotationMatrix::MakeFromX(GetMouseDirection()).Rotator();
	RETURN_IF_INVALID(IsValid(CachedEricaCharacter));
	CachedEricaCharacter->SetActorRotation(MouseDirectionRotator);
}

/**
 * 현재 마우스 커서의 위치를 반환합니다.\n
 * 실패 시 FVector::UnitX()를 반환합니다.
 * @return 현재 마우스 커서의 위치
 */
FVector ACEricaPlayerController::GetMouseDirection()
{
	FHitResult MouseClickedResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, MouseClickedResult))
	{
		FVector MouseClickedLocation = MouseClickedResult.Location;
		MouseClickedLocation.Z = 0.0;

		RETURN_IF_INVALID(CachedEricaCharacter, FVector::UnitX());
		FVector EricaRossLocation = CachedEricaCharacter->GetActorLocation();
		EricaRossLocation.Z = 0.0;

		FVector MouseDirection = (MouseClickedLocation - EricaRossLocation).GetSafeNormal();
		return MouseDirection;
	}

	return FVector::UnitX();
}

void ACEricaPlayerController::Shoot()
{
	GetMouseDirection();
	CachedEricaCharacter->ShootCard();
}

void ACEricaPlayerController::Return()
{
	CachedEricaCharacter->ReturnCard();
}
