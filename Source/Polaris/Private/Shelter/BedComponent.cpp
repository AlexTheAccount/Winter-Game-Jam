// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelter/BedComponent.h"
#include "Camera/PlayerCameraManager.h"
#include <FoodWater.h>
#include "EngineUtils.h"

// Sets default values for this component's properties
UBedComponent::UBedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBedComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBedComponent::StartSleeping(APlayerController* PlayerController, float HoursToSleep)
{
    if (!PlayerController || !GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("UBedComponent::StartSleeping - Invalid PlayerController or World"));
        return;
    }

    // Get the player's pawn
    APawn* PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBedComponent::StartSleeping - Invalid PlayerPawn"));
        return;
    }

    // Prevent repeated use while sleeping
    if (bIsSleeping)
        return;
    bIsSleeping = true;

    // Fade out quickly
    if (PlayerController->PlayerCameraManager)
    {
        PlayerController->PlayerCameraManager->StartCameraFade(0.f, 1.f, 0.5f, FLinearColor::Black, true, true);
    }

    // Disable player input for the duration
    PlayerPawn->DisableInput(PlayerController);

    // Run sleep completion after short delay for fade out
    const float FadeOutTime = 0.6f;
    FTimerHandle FinishHandle;
    GetWorld()->GetTimerManager().SetTimer(FinishHandle, [this, PlayerController, HoursToSleep, PlayerPawn]()
    {
        // Find TimeOfDayManager in the world (doesn't yet exist)
        // for (TActorIterator<ATimeOfDayManager> It(GetWorld()); It; ++It)
        // {
        //     ATimeOfDayManager* Manager = *It;
        //     if (Manager)
        //     {
        //         Manager->AdvanceTimeHours(HoursToSleep);
        //         break;
        //     }
        // }

        // heal, reduce fatigue
        if (UFoodWater* FoodWater = PlayerPawn->FindComponentByClass<UFoodWater>())
        {
            FoodWater->DecreaseHunger(10.0f * (HoursToSleep / 8.0f));
        }

        // Fade back in
        if (PlayerController->PlayerCameraManager)
        {
            PlayerController->PlayerCameraManager->StartCameraFade(1.f, 0.f, 0.6f, FLinearColor::Black, false, true);
        }

        // Re-enable input after fade-in
        FTimerHandle InputRestore;
        GetWorld()->GetTimerManager().SetTimer(InputRestore, [this, PlayerController, PlayerPawn]()
        {
            PlayerPawn->EnableInput(PlayerController);
            bIsSleeping = false;
        }, 0.65f, false);

    }, FadeOutTime, false);
}

// Called every frame
void UBedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

