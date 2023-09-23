// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/CMonsterBaseCharacter.h"

#include "Entities/Monsters/CMonsterDataAsset.h"
#include "UI/CUIDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"

ACMonsterBaseCharacter::ACMonsterBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealthPoint = 100.f;
	HealthPoint = MaxHealthPoint;
	AttackPower = 3.f;
	IsAlive = true;
	MoveSpeed = 300.f;
	DetectRange = 500.f;
	AttackRange = 100.f;

	bUseControllerRotationYaw = false;

	static ConstructorHelpers::FObjectFinder<UCMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.CMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCUIDataAsset> DA_UI(TEXT("/Script/ProjectCardReturn.CUIDataAsset'/Game/DataAssets/DA_UI.DA_UI'"));
	if (DA_UI.Succeeded())
	{
		UIDataAsset = DA_UI.Object;
	}

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	if (IsValid(HPBarWidgetComponent) && UIDataAsset)
	{
		HPBarWidgetComponent->SetupAttachment(RootComponent);
		HPBarWidgetComponent->SetRelativeLocation(FVector(0.0, 0.0, 200.0));
		HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		if (TSubclassOf<UUserWidget> HPBarClass = UIDataAsset->GetHPBar().LoadSynchronous())
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

void ACMonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHPChange.AddUObject(this, &ACMonsterBaseCharacter::HPChange);
	OnDead.AddUObject(this, &ACMonsterBaseCharacter::Dead);

	UUserWidget* HPBarWidgetInstance = HPBarWidgetComponent->GetUserWidgetObject();
	RETURN_IF_INVALID(HPBarWidgetInstance);
	HPProgressBar = Cast<UProgressBar>(HPBarWidgetInstance->GetWidgetFromName(TEXT("PB_HPBar")));
	RETURN_IF_INVALID(HPProgressBar);

	HPChange();
}

void ACMonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * 공격합니다.
 */
void ACMonsterBaseCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Monster's Attack!"));
}

/**
 * 공격을 받을 시 호출됩니다.
 * @param DamageAmount 받은 데미지량
 * @param DamageEvent 데미지 유형
 * @param EventInstigator 데미지를 가한 컨트롤러
 * @param DamageCauser 데미지를 가한 액터
 * @return 최종 받은 데미지량
 */
float ACMonsterBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("플레이어가 준 데미지: %f"), Damage);
	HealthPoint -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("남은 체력: %f"), HealthPoint);

	OnHPChange.Broadcast();
	return Damage;
}

/**
 * 데미지를 받아 체력이 변할때 호출되며 실시간으로 현재 체력과 HPBar를 동기화 해줍니다.\n
 * 호출시점은 TakeDamage 내부 참조하세요.
 */
void ACMonsterBaseCharacter::HPChange()
{
	if (HealthPoint <= 0.f)
	{
		HealthPoint = 0.f;
		OnDead.Broadcast();
	}

	RETURN_IF_INVALID(HPProgressBar);
	float HPRatio = HealthPoint / MaxHealthPoint;
	HPProgressBar->SetPercent(HPRatio);
}

/**
 * 죽으면 호출되며 몬스터를 지정된 시간 이후에 .
 * 호출시점은 HPChange 내부 참조하세요.
 */
void ACMonsterBaseCharacter::Dead()
{
	IsAlive = false;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, FTimerDelegate::CreateLambda([this]() -> void
	{
		Destroy();
	}),1.f /*TODO: 파라미터화 필요*/, false);
}

