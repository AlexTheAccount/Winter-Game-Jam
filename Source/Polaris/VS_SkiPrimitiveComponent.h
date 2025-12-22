// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/PrimitiveComponent.h"
#include "VS_SkiPrimitiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLARIS_API UVS_SkiPrimitiveComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
protected:
    // mesh component for the ski
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SkiMesh;

    // collision component for the ski
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* SkiCollision;

    // called on collision
    UFUNCTION()
    void OnSkiCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    // Sets default values for this component's properties
    UVS_SkiPrimitiveComponent();
};
