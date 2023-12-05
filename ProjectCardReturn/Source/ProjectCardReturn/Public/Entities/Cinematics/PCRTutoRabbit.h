// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRTutoRabbit.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimationSignature);

UCLASS()
class PROJECTCARDRETURN_API APCRTutoRabbit : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRTutoRabbit();

protected:
	virtual void PostInitializeComponents() override;
	UFUNCTION()

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FAnimationSignature OnAnimationEnd;

private:
	void StartToIdleEvent();
	
	UFUNCTION()
	void PlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayEndCallback(UAnimMontage* AnimMontage, bool bArg);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "SceneComponent")
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "TriggerBox")
	TObjectPtr<UBoxComponent> BoxComponent;

private: // 레퍼런스
	UPROPERTY(EditDefaultsOnly, Category = "Reference")
	TObjectPtr<UAnimMontage> TutoRabbitAnimMontage;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;
};
