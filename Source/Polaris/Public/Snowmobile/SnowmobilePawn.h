// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/BoxComponent.h>
#include "Components/StaticMeshComponent.h"
#include <Player/PolarisPlayerController.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "SnowmobilePawn.generated.h"

class UFoodWater;
class USkiSceneComponent;
class UTrackSceneComponent;
class UChassisComponent;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class POLARIS_API ASnowmobilePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnowmobilePawn();

    // References
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    APawn* PlayerPawn = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* PlayerMappingContext = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* SnowmobileMappingContext = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputAction* AccelerateAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputAction* BrakeAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputAction* DismountAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (AllowPrivateAccess = "true"))
    UInputAction* SteerAction = nullptr;

    // Health for FoodWater
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float Health = MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Input functions
    void Accelerate(const FInputActionValue& Value);
    void Steer(const FInputActionValue& Value);
    void Brake(const FInputActionValue& Value);
    void Dismount(const FInputActionValue& Value);

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* RootSceneComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UChassisComponent* ChassisComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkiSceneComponent* LeftSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkiSceneComponent* RightSkiComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTrackSceneComponent* TrackComponent = nullptr;

    // handle UFoodWater
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoodWater", meta = (AllowPrivateAccess = "true"))
    UFoodWater* FoodWaterComponent = nullptr;

    // meshes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* LeftSkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* RightSkiMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* TrackMesh = nullptr; 
    
    // collision
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* LeftSkiCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* RightSkiCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* TrackCollision = nullptr;

    // Physics constraints 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* LeftSkiConstraint = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* RightSkiConstraint = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* TrackConstraint = nullptr;


    // Seating config
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seating", meta = (AllowPrivateAccess = "true"))
    FVector SeatRelativeLocation = FVector(0.0f, 0.0f, 50.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seating", meta = (AllowPrivateAccess = "true"))
    FRotator SeatRelativeRotation = FRotator::ZeroRotator;

    // stabilization
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float StabilizationGain = 6000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float MaxStabilizationTorque = 120000.0f;

    // Steering tuning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float BaseAngularDamping = 240.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float SteeringAngularDamping = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float SteeringGain = 20.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float SteeringTorqueMultiplier = 10.5f;

    // Speed tuning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float MaxForwardSpeed = 6000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
    float AccelerationMultiplier = 5.0f;

    // Camera components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* SnowmobileCamera = nullptr;

    // FoodWater Handling Functions
    UFUNCTION()
    void HandlePlayerAppetite(float NewHunger, float NewThirst);

    UFUNCTION()
    void HandlePlayerStarving(float Damage);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    // Attachment
    void AttachPlayer(APawn* InPlayerPawn, APolarisPlayerController* PlayerController);
    void DetachPlayer(APolarisPlayerController* PlayerController);
    bool bIsPlayerAttached = false;

private:
    APolarisPlayerController* AttachedPlayerController = nullptr;

    // steering state
    bool bIsSteeringInputActive = false;
    float SavedAngularDamping = -1.0f;

    // previous view target for camera management
    AActor* PreviousViewTarget = nullptr;

    // Saved player rotation state
    bool SavedUseControllerRotationYaw = false;
    bool SavedOrientRotationToMovement = true;
    FRotator SavedControllerRotation = FRotator::ZeroRotator;
    FRotator SavedMeshRelativeRotation = FRotator::ZeroRotator;
    FRotator SavedActorRotation = FRotator::ZeroRotator;
    bool bHasSavedRotations = false;
};