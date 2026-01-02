// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelter/ShelterDoor.h"

// Sets default values
UShelterDoor::UShelterDoor()
{
    // Create components
    DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
}

// Called when the game starts or when spawned
void UShelterDoor::BeginPlay()
{
    // save rotation
    if (DoorMesh)
    {
        ClosedRotation = DoorMesh->GetRelativeRotation();
        OpenRotation = ClosedRotation + FRotator(0.f, OpenAngle, 0.f);
    }

    InitializeTimeline(DoorTimeline);
}

void UShelterDoor::InitializeTimeline(UTimelineComponent* InTimeline)
{
    DoorTimeline = InTimeline;
    if (DoorCurve && DoorTimeline)
    {
        FOnTimelineFloat Progress;
        Progress.BindUFunction(this, FName("TimelineProgress"));
        DoorTimeline->AddInterpFloat(DoorCurve, Progress);

        FOnTimelineEvent FinishedEvent;
        FinishedEvent.BindUFunction(this, FName("TimelineFinished"));
        DoorTimeline->SetTimelineFinishedFunc(FinishedEvent);

        DoorTimeline->SetLooping(false);
    }
}

void UShelterDoor::TimelineProgress(float Value)
{
    if (!DoorMesh)
    {
        return;
    }

    // interpolate rotation
    const FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Value);
    DoorMesh->SetRelativeRotation(NewRotation);
}

void UShelterDoor::TimelineFinished()
{
    // Update open state once finished
    bIsOpen = !bIsOpen;
}

void UShelterDoor::ToggleDoor()
{
    if (!DoorTimeline || !DoorCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleDoor called but DoorTimeline or DoorCurve is missing."));
        return;
    }

    if (DoorTimeline->IsPlaying())
    {
        // If currently playing, reverse direction
        DoorTimeline->Reverse();
        return;
    }

    if (bIsOpen)
    {
        // currently open, make close
        DoorTimeline->ReverseFromEnd();
    }
    else
    {
        // currently closed, make open
        DoorTimeline->PlayFromStart();
    }
}

void UShelterDoor::OpenDoor()
{
    if (!DoorTimeline || !DoorCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("OpenDoor called but DoorTimeline or DoorCurve is missing."));
        return;
    }

    if (bIsOpen)
    {
        // already open
        return;
    }

    bTargetOpen = true;

    if (DoorTimeline->IsPlaying())
    {
        // If timeline is playing toward closing, make it play forward
        DoorTimeline->Play();
    }
    else
    {
        DoorTimeline->PlayFromStart();
    }
}

void UShelterDoor::CloseDoor()
{
    if (!DoorTimeline || !DoorCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("CloseDoor called but DoorTimeline or DoorCurve is missing."));
        return;
    }

    if (!bIsOpen)
    {
        // already closed
        return;
    }

    bTargetOpen = false;

    if (DoorTimeline->IsPlaying())
    {
        // If timeline is playing toward opening, reverse it
        DoorTimeline->Reverse();
    }
    else
    {
        DoorTimeline->ReverseFromEnd();
    }
}