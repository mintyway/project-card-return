// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Cinematics/PCRTutoRabbit.h"

#include "Components/BoxComponent.h"
#include "Game/PCRGameModeBase.h"
#include "Kismet/GameplayStatics.h"

APCRTutoRabbit::APCRTutoRabbit()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    if (SceneComponent)
    {
        SetRootComponent(SceneComponent);
    }

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    if (MeshComponent)
    {
        MeshComponent->SetupAttachment(GetRootComponent());
    }

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    if (BoxComponent)
    {
        BoxComponent->SetupAttachment(MeshComponent);
        BoxComponent->SetGenerateOverlapEvents(true);
        BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
        BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
        BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    }
}

void APCRTutoRabbit::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void APCRTutoRabbit::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = MeshComponent->GetAnimInstance();
    check(AnimInstance);
    AnimInstance->Montage_Play(TutoRabbitAnimMontage);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &APCRTutoRabbit::StartToIdleEvent), 6.1f, false);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APCRTutoRabbit::PlayEnd);
}

void APCRTutoRabbit::StartToIdleEvent()
{
    APCRGameModeBase* PCRGameMode = Cast<APCRGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    check(PCRGameMode);
    PCRGameMode->ZoomOutRabbit();
}

void APCRTutoRabbit::PlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AnimInstance->Montage_JumpToSection(TEXT("End"), TutoRabbitAnimMontage);

    FOnMontageEnded MontageEnded;
    MontageEnded.BindUObject(this, &APCRTutoRabbit::PlayEndCallback);
    AnimInstance->Montage_SetEndDelegate(MontageEnded, TutoRabbitAnimMontage);
}

void APCRTutoRabbit::PlayEndCallback(UAnimMontage* AnimMontage, bool bArg)
{
    OnAnimationEnd.Broadcast();
    Destroy();
}
