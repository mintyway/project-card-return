// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRBaseItem.h"

#include "Components/BoxComponent.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

APCRBaseItem::APCRBaseItem()
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

void APCRBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRBaseItem::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard)
{
	check(AttachedCard);
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRBaseItem::HandleReturnCard);
}

void APCRBaseItem::HandleReturnCard(APCREricaCardProjectile* AttachedCard)
{
	AttachToActor(AttachedCard, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

