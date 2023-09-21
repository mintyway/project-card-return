// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "CProjectileDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UCProjectileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UStaticMesh> GetEricaCardMesh() const { return EricaCardMesh; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UStaticMesh> EricaCardMesh;
};
