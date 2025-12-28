// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/ChassisComponent.h"
#include "Polaris/Public/Snowmobile/SnowmobilePawn.h"
#include <EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include <Characters/PolarisFPSCharacter.h>

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
        SnowmobilePawn->PlayerPawn = OtherPawn;
        APolarisPlayerController* PlayerController = Cast<APolarisPlayerController>(OtherPawn->GetController());
        if (PlayerController && SnowmobilePawn)
        {
            // swap player to snowmobile controls
            SnowmobilePawn->AttachPlayer(OtherPawn, PlayerController);
        }
    }
}
