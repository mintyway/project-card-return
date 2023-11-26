// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Stage/Lift/PCRLiftActor.h"

#include "FMODBlueprintStatics.h"
#include "Components/BoxComponent.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Stage/Base/PCRStagePrimaryDataAsset.h"
#include "Game/PCRSoundPrimaryDataAsset.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PCRLogRiftActor);

APCRLiftActor::APCRLiftActor()
	: State(EState::LiftUp), MaxLiftHeight(900.f), LiftingTime(0.f), ElapsedTime(0.f), bIsOverlappedPattern1(false)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}
	
	Tags.AddUnique(TEXT("Lift"));
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(GetRootComponent());
		BoxComponent->SetBoxExtent(FVector(1000.0, 1400.0, 50.0));
		BoxComponent->SetRelativeLocation(FVector(0.0, 0.0, -50.0));
	}

	LiftMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LiftMeshComponent"));
	if (LiftMeshComponent)
	{
		LiftMeshComponent->SetupAttachment(GetRootComponent());
	}

	for (int32 i = 0; i < 4; ++i)
	{
		const FString ComponentKeyString = FString::Printf(TEXT("ColliderComponent%d"), i);
		const FName ComponentKey = *ComponentKeyString;
		ColliderComponents.Emplace(CreateDefaultSubobject<UBoxComponent>(ComponentKey));
		ColliderComponents[i]->SetupAttachment(GetRootComponent());
		ColliderComponents[i]->SetCollisionProfileName("NoCollision");
	}

	if (ColliderComponents[0])
	{
		const FVector NewLocation = FVector(0.0, -1400.0, 0.0);
		const FRotator NewRotation = FRotator(0.0, 90.0, 0.0);
		ColliderComponents[0]->SetRelativeLocationAndRotation(NewLocation, NewRotation);
		ColliderComponents[0]->InitBoxExtent(FVector(1.0, 1000.0, 1000.0));
	}

	if (ColliderComponents[1])
	{
		const FVector NewLocation = FVector(0.0, 1400.0, 0.0);
		const FRotator NewRotation = FRotator(0.0, -90.0, 0.0);
		ColliderComponents[1]->SetRelativeLocationAndRotation(NewLocation, NewRotation);
		ColliderComponents[1]->InitBoxExtent(FVector(1.0, 1000.0, 1000.0));
	}

	if (ColliderComponents[2])
	{
		const FVector NewLocation = FVector(1000.0, 0.0, 0.0);
		const FRotator NewRotation = FRotator(0.0, 180.0, 0.0);
		ColliderComponents[2]->SetRelativeLocationAndRotation(NewLocation, NewRotation);
		ColliderComponents[2]->InitBoxExtent(FVector(1.0, 1400.0, 1000.0));
	}

	if (ColliderComponents[3])
	{
		const FVector NewLocation = FVector(-1000.0, 0.0, 0.0);
		const FRotator NewRotation = FRotator(0.0, 0.0, 0.0);
		ColliderComponents[3]->SetRelativeLocationAndRotation(NewLocation, NewRotation);
		ColliderComponents[3]->InitBoxExtent(FVector(1.0, 1400.0, 1000.0));
	}

	Pattern1SuccessBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Pattern1SuccessBoxComponent"));
	if (Pattern1SuccessBoxComponent)
	{
		Pattern1SuccessBoxComponent->SetupAttachment(GetRootComponent());
		Pattern1SuccessBoxComponent->SetRelativeLocation(FVector(750.0, 0.0, 0.0));
		Pattern1SuccessBoxComponent->InitBoxExtent(FVector(300.0, 300.0, 500.0));
		Pattern1SuccessBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		Pattern1SuccessBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	}

	for (int32 i = 0; i < 6; ++i)
	{
		const FString ComponentKeyString = FString::Printf(TEXT("Patter1TargetComponent%d"), i);
		const FName ComponentKey = *ComponentKeyString;
		Pattern1TargetComponents.Emplace(CreateDefaultSubobject<USceneComponent>(ComponentKey));
		Pattern1TargetComponents[i]->SetupAttachment(GetRootComponent());
		Pattern1TargetComponents[i]->SetRelativeLocation(FVector(-800.0, -1125.0 + (450 * i), 0.0));
	}
}

void APCRLiftActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void APCRLiftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
		case EState::LiftUp:
		{
			UpdateLiftUp(DeltaTime);
			break;
		}
		case EState::SerinPattern1:
		{
			UpdatePattern1OverlapCheck();
			break;
		}
	}
}

void APCRLiftActor::LiftUp()
{
	const FFMODEventInstance LiftUpSound = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->StageChange, true);
	
	LiftUpSoundInst = LiftUpSound.Instance;
	check(LiftUpSoundInst);
	FMOD::Studio::EventDescription* Description = nullptr;
	LiftUpSoundInst->getDescription(&Description);
	int32 AudioLength;
	Description->getLength(&AudioLength);
	LiftingTime = AudioLength * 0.001f;
	
	SetActorTickEnabled(true);
	StartLiftLocation = GetActorLocation();
	EndLiftLocation = FVector(StartLiftLocation.X, StartLiftLocation.Y, MaxLiftHeight);
	State = EState::LiftUp;

	for (const auto& ColliderComponent : ColliderComponents)
	{
		ColliderComponent->SetCollisionProfileName("BlockAllDynamic");
	}
	
	OnLiftUpStart.Broadcast();
}

void APCRLiftActor::SerinPattern1Start()
{
	SIMPLE_LOG;
	TArray<AActor*> EricaArray;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),
	                                             APCREricaCharacter::StaticClass(),
	                                             TEXT("Erica"), EricaArray);
	CachedErica = Cast<APCREricaCharacter>(EricaArray[0]);
	check(CachedErica);

	SetActorTickEnabled(true);

	State = EState::SerinPattern1;
}

void APCRLiftActor::SerinPattern1End()
{
	UE_LOG(PCRLogRiftActor, Log, TEXT("SerinPattern1End"));
	SetActorTickEnabled(false);
}

TArray<FVector> APCRLiftActor::GetShuffleLocationForPattern1Target()
{
	TArray<int32> Indices;
	for (int32 i = 0; i < 6; ++i)
	{
		Indices.Add(i);
	}

	TArray<FVector> ShuffledPattern1TargetLocations;
	while (Indices.Num() > 0)
	{
		const int32 RandIndex = FMath::RandRange(0, Indices.Num() - 1);
		ShuffledPattern1TargetLocations.Add(Pattern1TargetComponents[Indices[RandIndex]]->GetComponentLocation());
		Indices.RemoveAt(RandIndex);
	}

	return ShuffledPattern1TargetLocations;
}

bool APCRLiftActor::IsOverlappedPattern1() const
{
	return bIsOverlappedPattern1;
}

void APCRLiftActor::UpdateLiftUp(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	const float alpha = FMath::Clamp(ElapsedTime / LiftingTime, 0.f, 1.f);
	const FVector NewLocation = FMath::Lerp(StartLiftLocation, EndLiftLocation, alpha);
	SetActorLocation(NewLocation);

	if (alpha >= 1.f)
	{
		LiftUpSoundInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
		LiftUpSoundInst->release();
		
		SetActorTickEnabled(false);
		OnLiftUpEnd.Broadcast();
	}
}

void APCRLiftActor::UpdatePattern1OverlapCheck()
{
	if (Pattern1SuccessBoxComponent->IsOverlappingActor(CachedErica))
	{
		bIsOverlappedPattern1 = true;
	}
	else
	{
		bIsOverlappedPattern1 = false;
	}
}
