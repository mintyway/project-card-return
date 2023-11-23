// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Game/PCRParameterDataAsset.h"
#include "UI/PCRUIDataAsset.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"

#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "BrainComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Entities/Item/PCRMoreHpItem.h"
#include "Entities/Item/PCRManyCardItem.h"
#include "Entities/Item/PCRHealItem.h"
#include "Entities/Item/PCRStrongAttackItem.h"
#include "Entities/Item/PCRLongerRangeItem.h"

DEFINE_LOG_CATEGORY(PCRLogMonsterBaseCharacter);

APCRMonsterBaseCharacter::APCRMonsterBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: 모두 파라미터화 필요
	MaxHP = 100.f;
	CurrentHP = MaxHP;
	MoveSpeed = 300.f;
	AttackPower = 3.f;
	AttackRange = 300.f;
	
	bIsAlive = true;
	bIsElite = false;

	bUseControllerRotationYaw = false;

	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRUIDataAsset> DA_UI(TEXT("/Script/ProjectCardReturn.PCRUIDataAsset'/Game/DataAssets/DA_UI.DA_UI'"));
	if (DA_UI.Succeeded())
	{
		UIDataAsset = DA_UI.Object;
	}

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	if (HPBarWidgetComponent && UIDataAsset)
	{
		HPBarWidgetComponent->SetupAttachment(RootComponent);
		HPBarWidgetComponent->SetRelativeLocation(FVector(0.0, 0.0, 200.0));
		HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		if (const TSubclassOf<UUserWidget> HPBarClass = UIDataAsset->HPBar.LoadSynchronous())
		{
			HPBarWidgetComponent->SetWidgetClass(HPBarClass);
			HPBarWidgetComponent->SetDrawSize(FVector2D(150.0, 15.0));
		}
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	}
}

void APCRMonsterBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnDestroyed.AddDynamic(this, &APCRMonsterBaseCharacter::PlayDeadEffect);
	OnDestroyed.AddDynamic(this, &APCRMonsterBaseCharacter::SpawnItem);
}

void APCRMonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 위젯의 포인터를 멤버변수로 가져오는 코드입니다.
	const UUserWidget* HPBarWidgetInstance = HPBarWidgetComponent->GetUserWidgetObject();
	check(HPBarWidgetInstance);
	HPProgressBar = Cast<UProgressBar>(HPBarWidgetInstance->GetWidgetFromName(TEXT("PB_HPBar")));
	check(HPProgressBar);

	// HP초기화를 위해 호출합니다.
	HandleChangeHP();
}

void APCRMonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 공격합니다.
 */
void APCRMonsterBaseCharacter::Attack() {}

/**
 * 매개변수 값에 따라 HP에 더하거나 빼며 변경시킵니다.
 * @param Amount 수정할 HP량(-30인 경우 HP30감소, 20인경우 HP20증가)
 */
void APCRMonsterBaseCharacter::ChangeHP(float Amount)
{
	CurrentHP += Amount;
	HandleChangeHP();
}

/**
 * 몬스터가 공격을 받을 시 호출됩니다.
 * @param DamageAmount 받은 데미지량
 * @param DamageEvent 데미지 유형
 * @param EventInstigator 데미지를 가한 컨트롤러
 * @param DamageCauser 데미지를 가한 액터
 * @return 최종 받은 데미지량
 */
float APCRMonsterBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// UE_LOG(PCRLogMonsterBaseCharacter, Warning, TEXT("플레이어가 준 데미지: %f"), Damage);
	ChangeHP(-ActualDamage);
	return ActualDamage;
}

/**
* 데미지를 받아 체력이 변할때 호출되며 실시간으로 현재 체력과 HPBar를 동기화 해줍니다. HP변경 후 무조건 이 함수를 호출하도록 설계해야합니다.\n
 * 이벤트가 존재합니다.
 */
void APCRMonsterBaseCharacter::HandleChangeHP()
{
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		HandleDead();
	}

	// UE_LOG(PCRLogMonsterBaseCharacter, Warning, TEXT("남은 체력: %f"), HealthPoint);
	check(HPProgressBar);
	const float HPRatio = CurrentHP / MaxHP;
	HPProgressBar->SetPercent(HPRatio);

	OnHPChange.Broadcast();
}

/**
 * 몬스터가 죽을때 호출되며 몬스터를 지정된 시간 이후에 제거합니다.\n
 * 이벤트가 존재합니다.
 */
void APCRMonsterBaseCharacter::HandleDead()
{
	UE_LOG(PCRLogMonsterBaseCharacter, Log, TEXT("%s가 죽었습니다."), *GetName());

	bIsAlive = false;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	APCRMonsterBaseAIController* const MonsterBaseAIController = Cast<APCRMonsterBaseAIController>(GetController());
	check(MonsterBaseAIController);
	MonsterBaseAIController->StopMovement();
	check(MonsterBaseAIController->GetBrainComponent());
	MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster is Dead");

	OnDead.Broadcast(this);
	
	check(ParameterDataAsset);
	FTimerHandle DestroyTimeHandle;
	FTimerDelegate DestroyTimeDelegate;
	DestroyTimeDelegate.BindUObject(this, &APCRMonsterBaseCharacter::DestroyTimeCallback);
	GetWorldTimerManager().SetTimer(DestroyTimeHandle, DestroyTimeDelegate, ParameterDataAsset->DeadAfterDestroyTime, false);
}

void APCRMonsterBaseCharacter::PlayDeadEffect(AActor* DestroyedActor)
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = 40;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MonsterDataAsset->DeadEffect, NewLocation, GetActorRotation());
}

void APCRMonsterBaseCharacter::SpawnItem(AActor* DestroyedActor)
{
	if (FMath::RandRange(1, 100) <= (!bIsElite ? ParameterDataAsset->ItemSpawnRate : ParameterDataAsset->ItemSpawnRateByEliteMonster) * 100)
	{
		GetWorld()->SpawnActor<APCRBaseItem>(GetItemClass(), GetActorLocation(), FRotator(0.0, 90.0, 0.0));
	}
}

UClass* APCRMonsterBaseCharacter::GetItemClass()
{
	const float MoreHpItemRate = ParameterDataAsset->MoreHpItemRate;
	const float ManyCardItemRate = ParameterDataAsset->ManyCardItemRate;
	const float HealItemRate = ParameterDataAsset->HealItemRate;
	const float StrongAttackItemRate = ParameterDataAsset->StrongAttackItemRate;
	const float LongerRangeItemRate = ParameterDataAsset->LongerRangeItemRate;

	const float RangeMax
		= MoreHpItemRate
		+ ManyCardItemRate
		+ HealItemRate
		+ StrongAttackItemRate
		+ LongerRangeItemRate;

	const int32 RandRange = FMath::RandRange(1, static_cast<int32>(RangeMax * 100));

	int32 Rate = MoreHpItemRate * 100;
	if (RandRange <= Rate)
	{
		return APCRMoreHpItem::StaticClass();
	}
	Rate += ManyCardItemRate * 100;

	if (RandRange <= Rate)
	{
		return APCRManyCardItem::StaticClass();
	}
	Rate += HealItemRate * 100;

	if (RandRange <= Rate)
	{
		return APCRHealItem::StaticClass();
	}
	Rate += StrongAttackItemRate * 100;

	if (RandRange <= Rate)
	{
		return APCRStrongAttackItem::StaticClass();
	}
	
	return APCRLongerRangeItem::StaticClass();
}

void APCRMonsterBaseCharacter::DestroyTimeCallback()
{
	Destroy();
}
