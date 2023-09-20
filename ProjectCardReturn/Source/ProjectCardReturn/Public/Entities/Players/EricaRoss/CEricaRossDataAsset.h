// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "CEricaRossDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 메인 플레이어블 캐릭터 에리카 로스에 필요한 에셋 모음입니다.
 */
UCLASS()
class PROJECTCARDRETURN_API UCEricaRossDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<USkeletalMesh> GetMesh() const { return Mesh; }
	FORCEINLINE TObjectPtr<UInputMappingContext> GetDefaultInputMappingContext() const { return DefaultInputMappingContext; }
	FORCEINLINE TObjectPtr<UInputAction> GetMoveInputAction() const { return MoveInputAction; }
	FORCEINLINE TObjectPtr<UInputAction> GetShootInputAction() const { return ShootInputAction; }
	FORCEINLINE TObjectPtr<UInputAction> GetReturnInputAction() const { return ReturnInputAction; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ReturnInputAction;
};