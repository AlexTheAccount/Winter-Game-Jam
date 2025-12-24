// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemperturePassive.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLARIS_API UTemperturePassive : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UTemperturePassive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnviromentTemp = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Insulation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FinalTemp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FreezingTickTemp = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLastFreezingTick = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimePerFreezingTick = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FreezingTickDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Freezing;

	UFUNCTION(BlueprintCallable)
	void CalculateTemp(float _DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
