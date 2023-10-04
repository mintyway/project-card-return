// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/ShieldActor.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

AShieldActor::AShieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh && MonsterDataAsset)
	{
		RootComponent = Mesh;
		Mesh->SetStaticMesh(MonsterDataAsset->ShieldMesh);
		Mesh->SetRelativeLocation(FVector(75.0, 0.0, 0.0));
		Mesh->SetCollisionProfileName("BlockPlayerProjectile");
	}
}

void AShieldActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

