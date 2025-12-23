// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_TrackSceneComponent.h"

UVS_TrackSceneComponent::UVS_TrackSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UVS_TrackSceneComponent::BeginPlay()
{
    if (TrackCollision)
    {
        if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(TrackCollision))
        {
            Primitive->OnComponentHit.AddDynamic(this, &UVS_TrackSceneComponent::OnTrackHit);
        }
    }
}

void UVS_TrackSceneComponent::OnTrackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherComponent && OtherComponent != HitComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Track hit actor: %s"), *OtherActor->GetName());
    }
}
