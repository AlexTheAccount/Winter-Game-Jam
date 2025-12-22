// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/PrimitiveComponent.h"
#include "VS_TrackPrimitiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLARIS_API UVS_TrackPrimitiveComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

protected:
    // mesh component for the track
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TrackMesh;

    // collision component for the track
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TrackCollision;

    // called on collision
    UFUNCTION()
    void OnTrackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    // Sets default values for this component's properties
    UVS_TrackPrimitiveComponent();
};
