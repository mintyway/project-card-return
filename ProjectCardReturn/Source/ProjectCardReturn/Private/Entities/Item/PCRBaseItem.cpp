// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRBaseItem.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Game/PCRParameterDataAsset.h"

APCRBaseItem::APCRBaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRItemDataAsset> DA_Item(TEXT("/Script/ProjectCardReturn.PCRItemDataAsset'/Game/DataAssets/DA_Item.DA_Item'"));
	if (DA_Item.Succeeded())
	{
		ItemDataAsset = DA_Item.Object;
	}
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetBoxExtent(FVector(10.0, 10.0, 10.0));
		BoxComponent->SetCollisionProfileName("Item");
	}

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	if (NiagaraComponent)
	{
		RootComponent = NiagaraComponent;
	}
}

void APCRBaseItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentHit.AddDynamic(this, &APCRBaseItem::HandleItemHit);

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
}

void APCRBaseItem::HandleItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APCREricaCharacter* Player = Cast<APCREricaCharacter>(OtherActor))
	{
		//
		
		Destroy();
	}
}

void APCRBaseItem::DestroyTimerCallback()
{
	Destroy();
}

void APCRBaseItem::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	check(AttachedCard);
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRBaseItem::HandleReturnCard);
}

