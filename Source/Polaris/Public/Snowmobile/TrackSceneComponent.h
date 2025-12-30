// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/SceneComponent.h"
#include "TrackSceneComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Snowmobile", meta = (BlueprintSpawnableComponent))
class POLARIS_API UTrackSceneComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    // Sets default values for this component's properties
    UTrackSceneComponent();

    // Track mesh (set by pawn)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References");
    UStaticMeshComponent* TrackMesh = nullptr;

    UFUNCTION()
    void OnTrackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnTrackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};
