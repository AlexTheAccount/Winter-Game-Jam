// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/SnowmobilePawn.h"
#include "Polaris/Public/Snowmobile/SkiSceneComponent.h"
#include "Polaris/Public/Snowmobile/TrackSceneComponent.h"
#include "Polaris/Public/Snowmobile/ChassisComponent.h"
#include "Polaris/Public/FoodWater.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

// Sets default values
ASnowmobilePawn::ASnowmobilePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    // Create components and set up attachment hierarchy

        // Root
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

        // Chassis and collision
    ChassisComponent = CreateDefaultSubobject<UChassisComponent>(TEXT("ChassisComponent"));
    ChassisComponent->SetupAttachment(RootSceneComponent);
    ChassisComponent->SetSimulatePhysics(true);

    ChassisCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ChassisCollision"));
    ChassisCollision->SetupAttachment(ChassisComponent);
    ChassisCollision->SetBoxExtent(ChassisCollisionExtent);
    ChassisCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        // Components for skis and track
    LeftSkiComponent = CreateDefaultSubobject<USkiSceneComponent>(TEXT("LeftSkiComponent"));
    LeftSkiComponent->SetupAttachment(ChassisComponent);

    RightSkiComponent = CreateDefaultSubobject<USkiSceneComponent>(TEXT("RightSkiComponent"));
    RightSkiComponent->SetupAttachment(ChassisComponent);

    TrackComponent = CreateDefaultSubobject<UTrackSceneComponent>(TEXT("TrackComponent"));
    TrackComponent->SetupAttachment(ChassisComponent);

        // Left Ski mesh and collision
    LeftSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSkiMesh"));
    LeftSkiMesh->SetupAttachment(LeftSkiComponent);

    LeftSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSkiCollision"));
    LeftSkiCollision->SetupAttachment(LeftSkiMesh);
    LeftSkiCollision->SetBoxExtent(SkiCollisionExtent);
    LeftSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LeftSkiCollision->SetGenerateOverlapEvents(true);

        // Right Ski mesh and collision
    RightSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSkiMesh"));
    RightSkiMesh->SetupAttachment(RightSkiComponent);

    RightSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSkiCollision"));
    RightSkiCollision->SetupAttachment(RightSkiMesh);
    RightSkiCollision->SetBoxExtent(SkiCollisionExtent);
    RightSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RightSkiCollision->SetGenerateOverlapEvents(true);

        // Track mesh and collision
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(TrackComponent);

    TrackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrackCollision"));
    TrackCollision->SetupAttachment(TrackMesh);
    TrackCollision->SetBoxExtent(TrackCollisionExtent);
    TrackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TrackCollision->SetGenerateOverlapEvents(true);
    TrackCollision->SetNotifyRigidBodyCollision(true);

        // Food and Water component
    FoodWaterComponent = CreateDefaultSubobject<UFoodWater>(TEXT("FoodWaterComponent"));
}

// Called when the game starts or when spawned
void ASnowmobilePawn::BeginPlay()
{
    Super::BeginPlay();

    // Set up component references

    if (ChassisComponent)
        ChassisComponent->SnowmobilePawn = this;

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

// Called to bind functionality to input
void ASnowmobilePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // input bindings
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ASnowmobilePawn::Accelerate);
        EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASnowmobilePawn::Steer);
        EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASnowmobilePawn::Brake);
        EnhancedInput->BindAction(DismountAction, ETriggerEvent::Triggered, this, &ASnowmobilePawn::Dismount);
    }
}

void ASnowmobilePawn::Accelerate(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    const FVector Force = GetActorForwardVector() * Value.Get<float>() * ChassisComponent->ForceMultiplier;
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
        Primitive->AddForce(Force, NAME_None, true);
}

void ASnowmobilePawn::Steer(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    const FVector Torque = FVector(0.f, 0.f, Value.Get<float>() * ChassisComponent->TorqueMultiplier);
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
        Primitive->AddTorqueInRadians(Torque, NAME_None, true);
}

void ASnowmobilePawn::Brake(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector CurrentVelocity = Primitive->GetPhysicsLinearVelocity();
        if (!CurrentVelocity.IsNearlyZero())
        {
            const FVector BrakeForce = -CurrentVelocity.GetSafeNormal() * Value.Get<float>() * ChassisComponent->ForceMultiplier;
            Primitive->AddForce(BrakeForce, NAME_None, true);
        }
    }
}

void ASnowmobilePawn::Dismount(const FInputActionValue& Value)
{
    APolarisPlayerController* PlayerController = Cast<APolarisPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (Value.Get<float>() > 0.5f && PlayerController)
    {
        PlayerPawn->SetActorHiddenInGame(false);
        PlayerController->Possess(PlayerPawn);

        // swap input to player pawn
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (PlayerMappingContext == nullptr || SnowmobileMappingContext == nullptr)
            {
                UE_LOG(LogTemp, Warning,
                    TEXT("ChassisComponent::OnChassisHit - Input Mapping Contexts not set on PlayerPawn"));
                return;
            }

            Subsystem->RemoveMappingContext(SnowmobileMappingContext);
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
        }
    }
}

void ASnowmobilePawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // copy food and water component from player pawn to snowmobile pawn
    if (PlayerPawn && FoodWaterComponent)
    {
        if (UFoodWater* PlayerFoodWater = PlayerPawn->FindComponentByClass<UFoodWater>())
        {
            // copy values
            FoodWaterComponent->Hunger = PlayerFoodWater->Hunger;
            FoodWaterComponent->Thirst = PlayerFoodWater->Thirst;
            FoodWaterComponent->MaxHunger = PlayerFoodWater->MaxHunger;
            FoodWaterComponent->MaxThirst = PlayerFoodWater->MaxThirst;
            FoodWaterComponent->HungerMult = PlayerFoodWater->HungerMult;
            FoodWaterComponent->ThirstMult = PlayerFoodWater->ThirstMult;
            FoodWaterComponent->StarveTickDamage = PlayerFoodWater->StarveTickDamage;
            FoodWaterComponent->TimeSinceLastStarveTick = PlayerFoodWater->TimeSinceLastStarveTick;

            // forward events
            PlayerFoodWater->OnAppetite.AddDynamic(this, &ASnowmobilePawn::HandlePlayerAppetite);
            PlayerFoodWater->OnStarving.AddDynamic(this, &ASnowmobilePawn::HandlePlayerStarving);

            // broadcast initial values
            FoodWaterComponent->OnAppetite.Broadcast(FoodWaterComponent->Hunger, FoodWaterComponent->Thirst);
        }
    }
}

void ASnowmobilePawn::UnPossessed()
{
    // remove bindings
    if (PlayerPawn)
    {
        if (UFoodWater* PlayerFoodWater = PlayerPawn->FindComponentByClass<UFoodWater>())
        {
            PlayerFoodWater->OnAppetite.RemoveDynamic(this, &ASnowmobilePawn::HandlePlayerAppetite);
            PlayerFoodWater->OnStarving.RemoveDynamic(this, &ASnowmobilePawn::HandlePlayerStarving);
        }
    }

    Super::UnPossessed();
}

void ASnowmobilePawn::HandlePlayerAppetite(float NewHunger, float NewThirst)
{
    if (!FoodWaterComponent)
        return;

    FoodWaterComponent->Hunger = NewHunger;
    FoodWaterComponent->Thirst = NewThirst;
    FoodWaterComponent->OnAppetite.Broadcast(NewHunger, NewThirst);
}

void ASnowmobilePawn::HandlePlayerStarving(float Damage)
{
    if (!FoodWaterComponent)
        return;

    FoodWaterComponent->OnStarving.Broadcast(Damage);
}