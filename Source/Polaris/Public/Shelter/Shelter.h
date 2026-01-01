// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include <Components/TimelineComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GameFramework/Actor.h"
#include "Shelter.generated.h"

class UShelterDoor;
class UShelterFuelComponent;
class UBedComponent;
UCLASS()
class POLARIS_API AShelter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShelter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnFuelStateChanged(bool bHasFuel);

    // components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UShelterDoor* ShelterDoor = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UShelterFuelComponent* FuelComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBedComponent* BedComponent = nullptr;

    // meshes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ShelterMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* FloorMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BedMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* FuelTankMesh = nullptr;

    // collision

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* FloorCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BedCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* FuelTankCollision = nullptr;

    // Animation
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
    UTimelineComponent* DoorTimeline;

private:
    // Construction helpers
    void InitializeComponents();
    void ConfigureMeshes();
    void ConfigureCollisions();
    void BindDelegates();
};
