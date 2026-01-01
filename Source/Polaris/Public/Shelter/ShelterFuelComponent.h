// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShelterFuelComponent.generated.h"

// Delegate for fuel state change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFuelStateChanged, bool, bHasFuel);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLARIS_API UShelterFuelComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShelterFuelComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
    float MaxFuel = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
    float Fuel = MaxFuel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
    float FuelConsumptionRate = 1.0f;

    UPROPERTY(BlueprintAssignable, Category = "Fuel")
    FOnFuelStateChanged OnFuelStateChanged;

    UFUNCTION()
    void OnFuelTankBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnFuelTankEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ConsumeFuel(float DeltaTime);
    void Refuel(float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    bool bPlayerInFuelingRange = false;

    bool bHadFuel = false;
    void BroadcastIfFuelStateChanged(bool bCurrentHasFuel);
};
