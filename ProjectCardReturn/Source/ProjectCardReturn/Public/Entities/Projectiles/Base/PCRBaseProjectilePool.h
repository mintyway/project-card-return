// Fill out your copyright notice in the Description page of Project Settings.

/* TODO: 단순히 객체를 저장, 반환하는 기능만 구현할 것. 객체의 비활성화는 해당 객체가 책임지고 할 것.
 * 풀이 바닥났을때 할 행동도 서브 클래스에서 구현할것.
 */


#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRBaseProjectilePool.generated.h"

class UPCRParameterDataAsset;
class APCRBaseProjectile;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogBaseProjectilePool, Log, All);

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRBaseProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRBaseProjectilePool();
	
public:
	virtual void InitProjectilePool(UClass* ProjectileClass);
	virtual APCRBaseProjectile* Acquire();
	virtual void Release(APCRBaseProjectile* Projectile);

protected:
	virtual APCRBaseProjectile* HandleEmptyPool();

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
	
	TQueue<TObjectPtr<APCRBaseProjectile>> ProjectilePool;
	TMap<APCRBaseProjectile*, FDelegateHandle> OnReleaseProjectileDelegateHandleMap;
	
	int32 ProjectilePoolSize;
};
