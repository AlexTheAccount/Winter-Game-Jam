// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/TrackSceneComponent.h"

UTrackSceneComponent::UTrackSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UTrackSceneComponent::BeginPlay()
{
    if (TrackCollision)
    {
        if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(TrackCollision))
        {
            Primitive->OnComponentHit.AddDynamic(this, &UTrackSceneComponent::OnTrackHit);
        }
    }
}

void UTrackSceneComponent::OnTrackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherComponent && OtherComponent != HitComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Track hit actor: %s"), *OtherActor->GetName());
    }
}
