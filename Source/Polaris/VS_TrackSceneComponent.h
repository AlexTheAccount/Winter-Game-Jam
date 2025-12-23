// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/SceneComponent.h"
#include "VS_TrackSceneComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Snowmobile", meta = (BlueprintSpawnableComponent))
class POLARIS_API UVS_TrackSceneComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTrackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
    // Sets default values for this component's properties
    UVS_TrackSceneComponent();

    // Track mesh and collision (set by pawn)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UStaticMeshComponent* TrackMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UBoxComponent* TrackCollision = nullptr;
};
