// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCREricaPlayerController.h"

#include "Entities/Players/Erica/PCREricaDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "UI/PCRUIDataAsset.h"
#include "UI/PCRMainUserWidget.h"
#include "UI/PCRPauseUserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PCRBossUserWidget.h"

APCREricaPlayerController::APCREricaPlayerController(): bUseCharacterRotationByCursorDirection(true)
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UPCREricaDataAsset> DA_Erica(TEXT("/Script/ProjectCardReturn.PCREricaDataAsset'/Game/DataAssets/DA_Erica.DA_Erica'"));
	if (DA_Erica.Succeeded())
	{
		EricaDataAsset = DA_Erica.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRUIDataAsset> DA_UI(TEXT("/Script/ProjectCardReturn.PCRUIDataAsset'/Game/DataAssets/DA_UI.DA_UI'"));
	if (DA_UI.Succeeded())
	{
		UIDataAsset = DA_UI.Object;
	}

	if (UIDataAsset)
	{
		if (const TSubclassOf<UPCRMainUserWidget> PCRMainUserWidgetClass = UIDataAsset->Main.LoadSynchronous())
		{
			MainUserWidgetClass = PCRMainUserWidgetClass;
		}
		if (const TSubclassOf<UPCRPauseUserWidget> PCRPauseUserWidgetClass = UIDataAsset->Pause.LoadSynchronous())
		{
			PauseUserWidgetClass = PCRPauseUserWidgetClass;
		}
		if (const TSubclassOf<UPCRBossUserWidget> PCRBossUserWidgetClass = UIDataAsset->Boss.LoadSynchronous())
		{
			BossUserWidgetClass = PCRBossUserWidgetClass;
		}
	}
}

void APCREricaPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 데이터 에셋의 유효성을 체크합니다. 이는 추후 nullptr 체크를 하지 않기 위해 여기서 체크합니다.
	check(EricaDataAsset);
	check(UIDataAsset);
}

void APCREricaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(EricaDataAsset->MenuInputAction, ETriggerEvent::Started, this, &APCREricaPlayerController::GamePause);
	}
}

void APCREricaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedEricaCharacter = Cast<APCREricaCharacter>(InPawn);
	check(CachedEricaCharacter);
	
	MainUserWidget = CreateWidget<UPCRMainUserWidget>(this, MainUserWidgetClass);
	check(MainUserWidget);
	MainUserWidget->AddToViewport(-1);

	CachedEricaCharacter->OnChangeHP.AddUObject(MainUserWidget, &UPCRMainUserWidget::HandleUpdateHP);
	CachedEricaCharacter->OnChangeCardCount.AddUObject(MainUserWidget, &UPCRMainUserWidget::HandleUpdateCardCount);
	CachedEricaCharacter->OnChangeShootMode.BindUObject(MainUserWidget, &UPCRMainUserWidget::HandleUpdateChangeShootMode);
	

	// UI 상태를 초기화 해줍니다.
	MainUserWidget->HandleUpdateHP(CachedEricaCharacter->GetMaxHP(), CachedEricaCharacter->GetCurrentHP());
	MainUserWidget->HandleUpdateCardCount(CachedEricaCharacter->GetMaxCardCount(), CachedEricaCharacter->GetCurrentCardCount());
}

void APCREricaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(EricaDataAsset->DefaultInputMappingContext, 0);
	}
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

		FVector EricaRossLocation = CachedEricaCharacter->GetActorLocation();
		EricaRossLocation.Z = 0.0;

		const FVector MouseDirection = (MouseClickedLocation - EricaRossLocation).GetSafeNormal();
		return MouseDirection;
	}

	return FVector::ZeroVector;
}

void APCREricaPlayerController::BindSerinUI(APCRSerinDollHeadCharacter* Serin)
{
	BossUserWidget = CreateWidget<UPCRBossUserWidget>(this, BossUserWidgetClass);
	check(BossUserWidget);
	BossUserWidget->AddToViewport(-1);
	
	Serin->OnChangeHP.AddUObject(BossUserWidget, &UPCRBossUserWidget::HandleUpdateHP);
	BossUserWidget->HandleUpdateHP(Serin->GetMaxHP(), Serin->GetCurrentHP());
}

void APCREricaPlayerController::GamePause()
{
	PauseUserWidget = CreateWidget<UPCRPauseUserWidget>(this, PauseUserWidgetClass);
	check(PauseUserWidget);

	PauseUserWidget->AddToViewport(3);

	SetPause(true);
}
