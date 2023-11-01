// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRBaseItem.h"

// Sets default values
APCRBaseItem::APCRBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCRBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APCRBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

