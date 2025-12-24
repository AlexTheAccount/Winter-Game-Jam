// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VS_SkiSceneComponent.h"
#include "VS_TrackSceneComponent.h"
#include "VS_ChassisComponent.h"
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* RootSceneComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UVS_ChassisComponent* ChassisComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* ChassisCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* LeftSkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* LeftSkiCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* RightSkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* RightSkiCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* TrackMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* TrackCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UVS_SkiSceneComponent* LeftSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UVS_SkiSceneComponent* RightSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UVS_TrackSceneComponent* TrackComponent = nullptr;

    // Collision Extents
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FVector ChassisCollisionExtent = FVector(100.f, 50.f, 25.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FVector SkiCollisionExtent = FVector(50.f, 10.f, 5.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    FVector TrackCollisionExtent = FVector(50.f, 10.f, 5.f);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
