// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "PolarisFPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class POLARIS_API APolarisFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APolarisFPSCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void FallProgress(float Value);
	
	UFUNCTION()
	void FallFinished();

	void GetUp();

	void FinishedGettingUp();
	
	void TraceForFloor(FHitResult& FloorHit);

protected:

	FTimeline FallTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY()
	FVector CameraStartLoc;
	UPROPERTY()
	FVector CameraEndLoc;
	UPROPERTY()
	FRotator CameraStartRot;
	UPROPERTY()
	FRotator CameraEndRot;

	UPROPERTY(EditAnywhere, Blueprintreadwrite, Category = "Timeline")
	float SlipThreshold;
	UPROPERTY()
	float SlipAmount;
	
	bool bIsFalling = false;
	bool bIsGettingUp = false;
	
	FTimerHandle GetUpTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
};
