// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RigidBodyBase.h"
#include "VS_ChassisStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Snowmobile", meta = (BlueprintSpawnableComponent))
class POLARIS_API UVS_ChassisStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
    UVS_ChassisStaticMeshComponent();

    // mesh component used as the physics body for the chassis (set by pawn)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
    UStaticMeshComponent* ChassisMesh;

    // multipliers for forces and torques
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ForceMultiplier = 500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float TorqueMultiplier = 500000.0f;
};
