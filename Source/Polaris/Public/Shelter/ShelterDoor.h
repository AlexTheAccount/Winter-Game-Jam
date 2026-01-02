// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Components/TimelineComponent.h>
#include "ShelterDoor.generated.h"

UCLASS()
class POLARIS_API UShelterDoor : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    UShelterDoor();

    void InitializeTimeline(UTimelineComponent* InTimeline);

    // Door control
    UFUNCTION(BlueprintCallable, Category = "Door")
    void ToggleDoor();

    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();

    UFUNCTION(BlueprintCallable, Category = "Door")
    void CloseDoor();

    void SetDoorMesh(UStaticMeshComponent* InDoorMesh) { DoorMesh = InDoorMesh; }
    void SetDoorCollision(UBoxComponent* InDoorCollision) { DoorCollision = InDoorCollision; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // references

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
    UTimelineComponent* DoorTimeline; 

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
    UCurveFloat* DoorCurve;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
    float OpenAngle = 135.0f;
    
    UFUNCTION() 
    void TimelineProgress(float Value); 
    
    UFUNCTION() 
    void TimelineFinished();

private:
    UStaticMeshComponent* DoorMesh = nullptr;
    UBoxComponent* DoorCollision = nullptr;

    FRotator ClosedRotation;
    FRotator OpenRotation;

    bool bIsOpen = false; 
    bool bTargetOpen = false;
};
