// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelter/ShelterFuelComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Player/PolarisPlayerController.h>

// Sets default values for this component's properties
UShelterFuelComponent::UShelterFuelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShelterFuelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UShelterFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Consume fuel over time
    ConsumeFuel(DeltaTime);

    if(bPlayerInFuelingRange)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if(PlayerPawn)
        {
            // Refuel at a rate of 10 units per second
            Refuel(10.0f * DeltaTime);
        }
    }
}


void UShelterFuelComponent::ConsumeFuel(float DeltaTime)
{
    Fuel -= FuelConsumptionRate * DeltaTime;
    Fuel = FMath::Clamp(Fuel, 0.0f, MaxFuel);

    // detect change in fuel state and broadcast
    const bool bHasFuel = (Fuel > 0.0f);
    BroadcastIfFuelStateChanged(bHasFuel);
}   

void UShelterFuelComponent::Refuel(float Amount)
{
    Fuel += Amount;
    Fuel = FMath::Clamp(Fuel, 0.0f, MaxFuel);

    // detect change in fuel state and broadcast
    const bool bHasFuel = (Fuel > 0.0f);
    BroadcastIfFuelStateChanged(bHasFuel);
}

void UShelterFuelComponent::BroadcastIfFuelStateChanged(bool bCurrentHasFuel)
{
    if (bCurrentHasFuel != bHadFuel)
    {
        bHadFuel = bCurrentHasFuel;
        OnFuelStateChanged.Broadcast(bCurrentHasFuel);
    }
}

void UShelterFuelComponent::OnFuelTankBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapping actor is the player and set the flag accordingly
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->GetController() && Pawn->GetController()->IsA(APolarisPlayerController::StaticClass()))
    {
        UE_LOG(LogTemp, Log, TEXT("Player entered fueling range"));
        bPlayerInFuelingRange = true;
    }
}

void UShelterFuelComponent::OnFuelTankEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Check if the overlapping actor is the player and set the flag accordingly
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->GetController() && Pawn->GetController()->IsA(APolarisPlayerController::StaticClass()))
    {
        UE_LOG(LogTemp, Log, TEXT("Player exited fueling range"));
        bPlayerInFuelingRange = false;
    }
}
