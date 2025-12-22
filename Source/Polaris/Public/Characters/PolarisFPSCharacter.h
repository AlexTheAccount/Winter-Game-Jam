// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PolarisFPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class POLARIS_API APolarisFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APolarisFPSCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FollowCamera;

protected:

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

};
