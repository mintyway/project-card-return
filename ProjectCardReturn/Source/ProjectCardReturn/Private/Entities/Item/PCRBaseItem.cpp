// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRBaseItem.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Game/PCRParameterDataAsset.h"

APCRBaseItem::APCRBaseItem() : bInteractCard(false)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRItemDataAsset> DA_Item(TEXT("/Script/ProjectCardReturn.PCRItemDataAsset'/Game/DataAssets/DA_Item.DA_Item'"));
	if (DA_Item.Succeeded())
	{
		ItemDataAsset = DA_Item.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	{
		RootComponent = NiagaraComponent;
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(NiagaraComponent);
		BoxComponent->SetBoxExtent(FVector(70.0, 70.0, 70.0));
		BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel10);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	}
}

void APCRBaseItem::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APCRBaseItem::HandleOverlap);
	
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &APCRBaseItem::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, ParameterDataAsset->ItemDestroyTime, false);
}

void APCRBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRBaseItem::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	AttachToActor(AttachedCard, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bInteractCard = false;
	
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &APCRBaseItem::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, ParameterDataAsset->ItemDestroyTime, false);
}

void APCRBaseItem::DestroyTimerCallback()
{
	if (!bInteractCard)
		Destroy();
}

void APCRBaseItem::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (APCREricaCharacter* Player = Cast<APCREricaCharacter>(OtherActor))
	{
		PlayerOverlapEvent(Player);
		Destroy();
	}
}

void APCRBaseItem::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	check(AttachedCard);
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRBaseItem::HandleReturnCard);
	bInteractCard = true;
}

void APCRBaseItem::PlayerOverlapEvent(APCREricaCharacter* Player)
{
	
}

