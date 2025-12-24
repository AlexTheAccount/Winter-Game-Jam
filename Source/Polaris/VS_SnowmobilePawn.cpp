// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_SnowmobilePawn.h"

// Sets default values
AVS_SnowmobilePawn::AVS_SnowmobilePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    // Create components and set up attachment hierarchy

    // Root
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Chassis and collision
    ChassisComponent = CreateDefaultSubobject<UVS_ChassisComponent>(TEXT("ChassisComponent"));
    ChassisComponent->SetupAttachment(RootSceneComponent);

    ChassisCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ChassisCollision"));
    ChassisCollision->SetupAttachment(ChassisComponent);
    ChassisCollision->SetBoxExtent(ChassisCollisionExtent);
    ChassisCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ChassisCollision->SetSimulatePhysics(true);

    // Components for skis and track
    LeftSkiComponent = CreateDefaultSubobject<UVS_SkiSceneComponent>(TEXT("LeftSkiComponent"));
    LeftSkiComponent->SetupAttachment(ChassisComponent);

    RightSkiComponent = CreateDefaultSubobject<UVS_SkiSceneComponent>(TEXT("RightSkiComponent"));
    RightSkiComponent->SetupAttachment(ChassisComponent);

    TrackComponent = CreateDefaultSubobject<UVS_TrackSceneComponent>(TEXT("TrackComponent"));
    TrackComponent->SetupAttachment(ChassisComponent);

    // Left Ski mesh and collision
    LeftSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSkiMesh"));
    LeftSkiMesh->SetupAttachment(LeftSkiComponent);
    LeftSkiMesh->SetSimulatePhysics(true);

    LeftSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSkiCollision"));
    LeftSkiCollision->SetupAttachment(LeftSkiMesh);
    LeftSkiCollision->SetBoxExtent(SkiCollisionExtent);
    LeftSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LeftSkiCollision->SetGenerateOverlapEvents(true);

    // Right Ski mesh and collision
    RightSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSkiMesh"));
    RightSkiMesh->SetupAttachment(RightSkiComponent);
    RightSkiMesh->SetSimulatePhysics(true);

    RightSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSkiCollision"));
    RightSkiCollision->SetupAttachment(RightSkiMesh);
    RightSkiCollision->SetBoxExtent(SkiCollisionExtent);
    RightSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RightSkiCollision->SetGenerateOverlapEvents(true);

    // Track mesh and collision
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(TrackComponent);
    TrackMesh->SetSimulatePhysics(true);

    TrackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrackCollision"));
    TrackCollision->SetupAttachment(TrackMesh);
    TrackCollision->SetBoxExtent(TrackCollisionExtent);
    TrackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TrackCollision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AVS_SnowmobilePawn::BeginPlay()
{
    Super::BeginPlay();

    // Set up component references
    if (LeftSkiComponent)
    {
        LeftSkiComponent->Chassis = ChassisComponent;
        LeftSkiComponent->SkiMesh = LeftSkiMesh;
        LeftSkiComponent->SkiCollision = LeftSkiCollision;
    }

    if (RightSkiComponent)
    {
        RightSkiComponent->Chassis = ChassisComponent;
        RightSkiComponent->SkiMesh = RightSkiMesh;
        RightSkiComponent->SkiCollision = RightSkiCollision;
    }

    if (TrackComponent)
    {
        TrackComponent->TrackMesh = TrackMesh;
        TrackComponent->TrackCollision = TrackCollision;
    }
}

// Called every frame
void AVS_SnowmobilePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVS_SnowmobilePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // input bindings
    PlayerInputComponent->BindAxis("Accelerate", this, &AVS_SnowmobilePawn::Accelerate);
    PlayerInputComponent->BindAxis("Steer", this, &AVS_SnowmobilePawn::Steer);
    PlayerInputComponent->BindAxis("Brake", this, &AVS_SnowmobilePawn::Brake);
}

void AVS_SnowmobilePawn::Accelerate(float Value)
{
    if (FMath::Abs(Value) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    const FVector Force = GetActorForwardVector() * Value * ChassisComponent->ForceMultiplier;
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
        Primitive->AddForce(Force, NAME_None, true);
}

void AVS_SnowmobilePawn::Steer(float Value)
{
    if (FMath::Abs(Value) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    const FVector Torque = FVector(0.f, 0.f, Value * ChassisComponent->TorqueMultiplier);
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
        Primitive->AddTorqueInRadians(Torque, NAME_None, true);
}

void AVS_SnowmobilePawn::Brake(float Value)
{
    if (FMath::Abs(Value) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector CurrentVelocity = Primitive->GetPhysicsLinearVelocity();
        if (!CurrentVelocity.IsNearlyZero())
        {
            const FVector BrakeForce = -CurrentVelocity.GetSafeNormal() * Value * ChassisComponent->ForceMultiplier;
            Primitive->AddForce(BrakeForce, NAME_None, true);
        }
    }
}

