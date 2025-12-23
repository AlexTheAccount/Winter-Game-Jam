// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_SnowmobilePawn.h"

// Sets default values
AVS_SnowmobilePawn::AVS_SnowmobilePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    // Create components and set up attachment hierarchy
    ChassisComponent = CreateDefaultSubobject<UVS_ChassisStaticMeshComponent>(TEXT("ChassisComponent"));
    RootComponent = ChassisComponent;

    // Chassis mesh (physics body)
    ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChassisMesh"));
    ChassisMesh->SetupAttachment(ChassisComponent);
    ChassisMesh->SetSimulatePhysics(true);
    ChassisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ChassisMesh->SetCollisionObjectType(ECC_PhysicsBody);
    ChassisMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ChassisMesh->SetMobility(EComponentMobility::Movable);

    // Ski mesh and collision
    SkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkiMesh"));
    SkiMesh->SetupAttachment(ChassisComponent);
    SkiMesh->SetSimulatePhysics(true);

    SkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SkiCollision"));
    SkiCollision->SetupAttachment(SkiMesh);
    SkiCollision->SetBoxExtent(SkiCollisionExtent);
    SkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SkiCollision->SetGenerateOverlapEvents(true);

    // Track mesh and collision
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(ChassisComponent);
    TrackMesh->SetSimulatePhysics(true);

    TrackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrackCollision"));
    TrackCollision->SetupAttachment(TrackMesh);
    TrackCollision->SetBoxExtent(TrackCollisionExtent);
    TrackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TrackCollision->SetGenerateOverlapEvents(true);

    // Components for skis and track
    LeftSkiComponent = CreateDefaultSubobject<UVS_SkiSceneComponent>(TEXT("LeftSkiComponent"));
    LeftSkiComponent->SetupAttachment(ChassisComponent);

    RightSkiComponent = CreateDefaultSubobject<UVS_SkiSceneComponent>(TEXT("RightSkiComponent"));
    RightSkiComponent->SetupAttachment(ChassisComponent);

    TrackComponent = CreateDefaultSubobject<UVS_TrackSceneComponent>(TEXT("TrackComponent"));
    TrackComponent->SetupAttachment(ChassisComponent);
}

// Called when the game starts or when spawned
void AVS_SnowmobilePawn::BeginPlay()
{
    Super::BeginPlay();

    // Set up component references
    if (ChassisComponent)
        ChassisComponent->ChassisMesh = ChassisMesh;

    if (LeftSkiComponent)
    {
        LeftSkiComponent->Chassis = ChassisComponent;
        LeftSkiComponent->SkiMesh = SkiMesh;
        LeftSkiComponent->SkiCollision = SkiCollision;
    }

    if (RightSkiComponent)
    {
        RightSkiComponent->Chassis = ChassisComponent;
        RightSkiComponent->SkiMesh = SkiMesh;
        RightSkiComponent->SkiCollision = SkiCollision;
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

