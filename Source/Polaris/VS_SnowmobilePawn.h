// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VS_SkiPrimitiveComponent.h"
#include "VS_TrackPrimitiveComponent.h"
#include "VS_Snowmobile_RBB_Component.h"
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

    // components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCharacterMovementComponent* MovementComponent;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    AVS_Snowmobile_RBB_Component* RBBComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_SkiPrimitiveComponent* LeftSkiComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_SkiPrimitiveComponent* RightSkiComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UVS_TrackPrimitiveComponent* TrackComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
