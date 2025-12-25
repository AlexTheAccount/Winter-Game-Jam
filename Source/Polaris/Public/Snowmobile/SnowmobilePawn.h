// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/BoxComponent.h>
#include "Components/StaticMeshComponent.h"
#include <Player/PolarisPlayerController.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "SnowmobilePawn.generated.h"

class USkiSceneComponent;
class UTrackSceneComponent;
class UChassisComponent;
UCLASS()
class POLARIS_API ASnowmobilePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnowmobilePawn();

    // References
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
    APolarisPlayerController* PlayerController = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Input functions
    void Accelerate(float Value);
    void Steer(float Value);
    void Brake(float Value);
    void Dismount(float Value);

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* RootSceneComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UChassisComponent* ChassisComponent = nullptr;

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
    USkiSceneComponent* LeftSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkiSceneComponent* RightSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTrackSceneComponent* TrackComponent = nullptr;

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
