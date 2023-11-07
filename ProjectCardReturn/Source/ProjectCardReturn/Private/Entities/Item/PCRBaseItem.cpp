// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRBaseItem.h"

#include "Components/BoxComponent.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"

APCRBaseItem::APCRBaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent)
	{
		RootComponent = BoxComponent;
		BoxComponent->SetBoxExtent(FVector(10.0, 10.0, 10.0));
		BoxComponent->SetCollisionProfileName("Item");
	}
}

void APCRBaseItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentHit.AddDynamic(this, &APCRBaseItem::HandleItemHit);
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

void APCRBaseItem::HandleItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APCREricaCharacter* Player = Cast<APCREricaCharacter>(OtherActor))
	{
		
	}
}

