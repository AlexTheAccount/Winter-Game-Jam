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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Input functions
    void Accelerate(const FInputActionValue& Value);
    void Steer(const FInputActionValue& Value);
    void Brake(const FInputActionValue& Value);
    void Dismount(const FInputActionValue& Value);

    // Possession
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;

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

    // handle UFoodWater
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UFoodWater* FoodWaterComponent = nullptr;

    UFUNCTION()
    void HandlePlayerAppetite(float NewHunger, float NewThirst);

    UFUNCTION()
    void HandlePlayerStarving(float Damage);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
