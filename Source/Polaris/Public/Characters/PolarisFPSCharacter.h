// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "PolarisFPSCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQTEEnd, bool /*QTE Active */)
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;

UCLASS()
class POLARIS_API APolarisFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APolarisFPSCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	

protected:
	
	void TraceForFloor(FHitResult& FloorHit);
	UFUNCTION()
	void FallProgress(float Value);
	UFUNCTION()
	void FallFinished();
	void StartFall();
	void GetUp();
	void FinishedGettingUp();
	void StartQTE();
	void EndQTE();
	void ResetQTEValues();

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
	
	FTimerHandle GetUpTimer;
	
	bool bIsFalling = false;
	bool bIsGettingUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline|QTE")
	float QTEEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline|QTE")
	bool bQTESuccess = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline|QTE")
	bool bQTEActive = false;
	
	FTimerHandle QTETimer;

	UFUNCTION(BlueprintCallable)
	void QTESuccessed();

	TObjectPtr<UUserWidget> QTEWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline|QTE")
	TSubclassOf<UUserWidget> QTEClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
};
