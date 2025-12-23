// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VS_SkiSceneComponent.h"
#include "VS_TrackSceneComponent.h"
#include "VS_ChassisStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VS_SnowmobilePawn.generated.h"

UCLASS()
class POLARIS_API AVS_SnowmobilePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVS_SnowmobilePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Input functions
    void Accelerate(float Value);
    void Steer(float Value);
    void Brake(float Value);

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_ChassisStaticMeshComponent* ChassisComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ChassisMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* SkiCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TrackMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TrackCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_SkiSceneComponent* LeftSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_SkiSceneComponent* RightSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_TrackSceneComponent* TrackComponent = nullptr;

    // Collision Extents
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FVector SkiCollisionExtent = FVector(50.f, 10.f, 5.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FVector TrackCollisionExtent = FVector(50.f, 10.f, 5.f);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
