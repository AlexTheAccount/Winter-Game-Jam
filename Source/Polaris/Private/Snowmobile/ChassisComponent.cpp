// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/ChassisComponent.h"
#include "Polaris/Public/Snowmobile/SnowmobilePawn.h"
#include <EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>

UChassisComponent::UChassisComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UChassisComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(this))
    {
        Primitive->OnComponentHit.AddDynamic(this, &UChassisComponent::OnChassisHit);
    }
}

void UChassisComponent::OnChassisHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // attach player to snowmobile on first hit
    APawn* OtherPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (OtherPawn && Cast<APawn>(OtherActor) == OtherPawn)
    {
        OtherPawn->SetActorHiddenInGame(true);
        SnowmobilePawn->PlayerPawn = OtherPawn;
        APolarisPlayerController* PlayerController = Cast<APolarisPlayerController>(OtherPawn->GetController());

        PlayerController->Possess(SnowmobilePawn);

        // swap input to snowmobile
        if(UEnhancedInputLocalPlayerSubsystem* Subsystem = 
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (SnowmobilePawn->PlayerMappingContext == nullptr || SnowmobilePawn->SnowmobileMappingContext == nullptr)
            {
                UE_LOG(LogTemp, Warning, 
                    TEXT("ChassisComponent::OnChassisHit - Input Mapping Contexts not set on SnowmobilePawn"));
                return;
            }

            Subsystem->RemoveMappingContext(SnowmobilePawn->PlayerMappingContext);
            Subsystem->AddMappingContext(SnowmobilePawn->SnowmobileMappingContext, 0);
        }
    }
}
