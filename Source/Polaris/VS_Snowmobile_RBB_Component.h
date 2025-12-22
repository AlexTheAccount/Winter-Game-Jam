// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RigidBodyBase.h"
#include "VS_Snowmobile_RBB_Component.generated.h"

/**
 * 
 */
UCLASS()
class POLARIS_API AVS_Snowmobile_RBB_Component : public ARigidBodyBase
{
	GENERATED_BODY()

protected:
    // mesh component for the chassis
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ChassisMesh;

public:
    // Sets default values for this component's properties
    AVS_Snowmobile_RBB_Component();
};
