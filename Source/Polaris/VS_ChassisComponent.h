// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/StaticMeshComponent.h>
#include "VS_ChassisComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Snowmobile", meta = (BlueprintSpawnableComponent))
class POLARIS_API UVS_ChassisComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
    UVS_ChassisComponent();

    // fuel amount
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float Fuel = 500.0f;

    // multipliers for forces and torques
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ForceMultiplier = 500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float TorqueMultiplier = 500000.0f;
};
