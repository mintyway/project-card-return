// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRItem.h"

#include "Components/BoxComponent.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

APCRItem::APCRItem()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetBoxExtent(FVector(10.0, 10.0, 10.0));
		BoxComponent->SetCollisionProfileName("BlockPlayerProjectile");
	}
}

void APCRItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRItem::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	check(AttachedCard);
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRItem::HandleReturnCard);
}

void APCRItem::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	AttachToActor(AttachedCard, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

