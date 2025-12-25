// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/ChassisComponent.h"
#include "Polaris/Public/Snowmobile/SnowmobilePawn.h"

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
    if (OtherActor && OtherComponent && OtherComponent != HitComponent)
    {
        if(OtherActor == SnowmobilePawn->PlayerController)
        {
            SnowmobilePawn->PlayerController->Possess(SnowmobilePawn);
        }
    }
}
