// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "CEricaDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 메인 플레이어블 캐릭터 에리카 로스에 필요한 에셋 모음입니다.
 */
UCLASS()
class PROJECTCARDRETURN_API UCEricaDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE USkeletalMesh* GetSkeletalMesh() const { return SkeletalMesh; }
	FORCEINLINE UInputMappingContext* GetDefaultInputMappingContext() const { return DefaultInputMappingContext; }
	FORCEINLINE UInputAction* GetMoveInputAction() const { return MoveInputAction; }
	FORCEINLINE UInputAction* GetShootInputAction() const { return ShootInputAction; }
	FORCEINLINE UInputAction* GetReturnInputAction() const { return ReturnInputAction; }
	FORCEINLINE UInputAction* GetDashInputAction() const { return DashInputAction; }
	FORCEINLINE UInputAction* GetChangeInputAction() const { return ChangeInputAction; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ReturnInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> DashInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ChangeInputAction;
};