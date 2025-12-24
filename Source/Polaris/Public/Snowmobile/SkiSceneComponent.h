// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/SceneComponent.h"
#include "ChassisComponent.h"
#include "SkiSceneComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Snowmobile", meta = (BlueprintSpawnableComponent))
class POLARIS_API USkiSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
    USkiSceneComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Snowmobile chassis reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UChassisComponent* Chassis;

    // Ski mesh and collision (set by pawn)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UStaticMeshComponent* SkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UBoxComponent* SkiCollision = nullptr;

protected:
    // Compute lateral velocity on contact
    FVector ComputeLateralVelocity(const FHitResult& Hit) const;

    // Raycast parameters 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float MaxTraceDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float RestLength = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float SpringStiffness = 100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float DampingCoefficient = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float LateralStiffness = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float SkiFriction = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast", meta = (ClampMin = "0.0"))
    float DampenerMultiplier = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast", meta = (ClampMin = "0.0"))
    float StaticFrictionThreshold = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    bool bDrawTraceDebug = false;

private:
    // apply forces from ski to chassis
    void ApplySkiForces(const FHitResult& Hit);
};
