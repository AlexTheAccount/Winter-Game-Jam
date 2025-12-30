// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoodWater.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCastAppetite, float, NewHunger, float, NewThirst);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastStarving, float, damage);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLARIS_API UFoodWater : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UFoodWater();

	UPROPERTY(BlueprintAssignable)
	FCastAppetite OnAppetite;

	UPROPERTY(BlueprintAssignable)
	FCastStarving OnStarving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float MaxHunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float MaxThirst = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float Hunger = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float Thirst = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float HungerMult = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float ThirstMult = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float TimePerStarveTick = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VALUE")
	float TimeSinceLastStarveTick = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float StarveTickDamage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	bool Thirsty;
	

	UFUNCTION(BlueprintCallable)
	void CalculateAppetite(float _DeltaTime);

	UFUNCTION(BlueprintCallable)
	void DecreaseHunger(float _PercentToDecrease);

	UFUNCTION(BlueprintCallable)
	void IncreaseHunger(float _PercentToIncrease);
	

	UFUNCTION(BlueprintCallable)
	void DecreaseThirst(float _PercentToDecrease);

	UFUNCTION(BlueprintCallable)
	void IncreaseThirst(float _PercentToIncrease);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
