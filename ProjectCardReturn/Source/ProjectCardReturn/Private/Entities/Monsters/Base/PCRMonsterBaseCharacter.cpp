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

DEFINE_LOG_CATEGORY(PCRLogMonsterBaseCharacter);

APCRMonsterBaseCharacter::APCRMonsterBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: 모두 파라미터화 필요
	MaxHP = 100.f;
	CurrentHP = MaxHP;
	AttackPower = 3.f;
	bIsAlive = true;
	MoveSpeed = 300.f;
	AttackRange = 300.f;
	StunTime = 1.f;

	bUseControllerRotationYaw = false;

	AIControllerClass = APCRMonsterBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

	if (ParameterDataAsset)
	{
		HitStopTime = ParameterDataAsset->HitStopTime;
	}

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	if (IsValid(HPBarWidgetComponent) && IsValid(UIDataAsset))
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

	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	}
}

void APCRMonsterBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APCRMonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 위젯의 포인터를 멤버변수로 가져오는 코드입니다.
	const UUserWidget* HPBarWidgetInstance = HPBarWidgetComponent->GetUserWidgetObject();
	RETURN_IF_INVALID(HPBarWidgetInstance);
	HPProgressBar = Cast<UProgressBar>(HPBarWidgetInstance->GetWidgetFromName(TEXT("PB_HPBar")));
	RETURN_IF_INVALID(HPProgressBar);

	// HP초기화를 위해 호출합니다.
	HandleChangeHP();
}

void APCRMonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * 공격합니다.
 * TODO: 구현 필요
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
 * 피격 시 경직됩니다.
 */
void APCRMonsterBaseCharacter::HitStop()
{
	APCRMonsterBaseAIController* MonsterBaseAIController = Cast<APCRMonsterBaseAIController>(GetController());
	check(MonsterBaseAIController);
	MonsterBaseAIController->ApplyStun(HitStopTime);
}

/**
 * 잠시동안 스턴에 빠집니다.
 */
void APCRMonsterBaseCharacter::Stun()
{
	APCRMonsterBaseAIController* MonsterBaseAIController = Cast<APCRMonsterBaseAIController>(GetController());
	check(MonsterBaseAIController);
	MonsterBaseAIController->ApplyStun(StunTime);
	UE_LOG(PCRLogMonsterBaseAIController, Log, TEXT("%s가 %f.1초간 스턴에 빠집니다."), *this->GetName(), StunTime);
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

	// TODO: 현재는 데미지 0을 스턴으로 활용하고 있지만 추후 인터페이스를 통해 전달하도록 변경 필요
	if (ActualDamage <= 0)
	{
		UE_LOG(PCRLogMonsterBaseCharacter, Log, TEXT("%s가 %s에게 상태이상 공격을 당했습니다."), *this->GetName(), *DamageCauser->GetName());

		Stun();
		return ActualDamage;
	}

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

	RETURN_IF_INVALID(HPProgressBar);
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
	const auto MonsterBaseAIController = Cast<APCRMonsterBaseAIController>(GetController());
	check(MonsterBaseAIController && MonsterBaseAIController->GetBrainComponent());
	MonsterBaseAIController->StopMovement();
	MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster is Dead");

	RETURN_IF_INVALID(IsValid(ParameterDataAsset));
	FTimerHandle DestroyTimeHandle;
	FTimerDelegate DestroyTimeDelegate;
	DestroyTimeDelegate.BindUObject(this, &APCRMonsterBaseCharacter::DestroyTimeCallback);
	GetWorldTimerManager().SetTimer(DestroyTimeHandle, DestroyTimeDelegate, ParameterDataAsset->DeadAfterDestroyTime, false);
	
	OnDead.Broadcast(this);
}

void APCRMonsterBaseCharacter::DestroyTimeCallback()
{
	Destroy();
}
