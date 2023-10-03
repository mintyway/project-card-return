// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaPlayerController.h"

#include "Entities/Players/Erica/PCREricaDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APCREricaPlayerController::APCREricaPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UPCREricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.PCREricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		DataAsset = DA_Erica.Object;
	}

	bShowMouseCursor = true;
}

void APCREricaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		RETURN_IF_INVALID(DataAsset);
		EnhancedInputComponent->BindAction(DataAsset->ShootInputAction, ETriggerEvent::Triggered, this, &APCREricaPlayerController::Shoot);
		EnhancedInputComponent->BindAction(DataAsset->ReturnInputAction, ETriggerEvent::Triggered, this, &APCREricaPlayerController::Return);
	}
}

void APCREricaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedEricaCharacter = Cast<APCREricaCharacter>(InPawn);
	RETURN_IF_INVALID(CachedEricaCharacter);
}

void APCREricaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		RETURN_IF_INVALID(IsValid(DataAsset));
		Subsystem->AddMappingContext(DataAsset->DefaultInputMappingContext, 0);
	}
}

void APCREricaPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FRotator MouseDirectionRotator = FRotationMatrix::MakeFromX(GetMouseDirection()).Rotator();
	RETURN_IF_INVALID(IsValid(CachedEricaCharacter));
	CachedEricaCharacter->SetActorRotation(MouseDirectionRotator);
}

/**
 * 현재 마우스 커서의 위치를 향하는 방향 벡터를 반환합니다.\n
 * 실패 시 FVector::ZeroVector를 반환합니다.
 * @return 현재 마우스 커서의 위치
 */
FVector APCREricaPlayerController::GetMouseDirection() const
{
	FHitResult MouseClickedResult;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel5, false, MouseClickedResult))
	{
		FVector MouseClickedLocation = MouseClickedResult.Location;
		MouseClickedLocation.Z = 0.0;
	
		RETURN_IF_INVALID(CachedEricaCharacter, FVector::ZeroVector);
		FVector EricaRossLocation = CachedEricaCharacter->GetActorLocation();
		EricaRossLocation.Z = 0.0;
	
		FVector MouseDirection = (MouseClickedLocation - EricaRossLocation).GetSafeNormal();
		return MouseDirection;
	}

	return FVector::ZeroVector;
}

void APCREricaPlayerController::Shoot()
{
	CachedEricaCharacter->ShootCard();
}

void APCREricaPlayerController::Return()
{
	CachedEricaCharacter->ReturnCard();
}
