// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/TrackSceneComponent.h"

UTrackSceneComponent::UTrackSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UTrackSceneComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTrackSceneComponent::OnTrackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherComponent && OtherComponent != HitComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Track hit actor: %s"), *OtherActor->GetName());
    }
}

void UTrackSceneComponent::OnTrackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool 
    bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Track begin overlap invalid actor or component"));

        return;
    }

    // If sweep contains blocking info
    if (bFromSweep && SweepResult.bBlockingHit)
    {
        OnTrackHit(OverlappedComponent, OtherActor, OtherComp, FVector::ZeroVector, SweepResult);
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("Track overlap with %s"), *OtherActor->GetName());
    }
}
