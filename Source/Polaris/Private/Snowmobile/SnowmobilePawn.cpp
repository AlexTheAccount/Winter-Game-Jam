// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/SnowmobilePawn.h"
#include "Polaris/Public/Snowmobile/SkiSceneComponent.h"
#include "Polaris/Public/Snowmobile/TrackSceneComponent.h"
#include "Polaris/Public/Snowmobile/ChassisComponent.h"
#include "Polaris/Public/FoodWater.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
#include <Characters/PolarisFPSCharacter.h>
#include "Components/CapsuleComponent.h"

// Sets default values
ASnowmobilePawn::ASnowmobilePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    // Create components and set up attachment hierarchy

        // Root
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

        // Chassis
    ChassisComponent = CreateDefaultSubobject<UChassisComponent>(TEXT("ChassisComponent"));
    ChassisComponent->SetupAttachment(RootSceneComponent);
    ChassisComponent->SetSimulatePhysics(true);

        // Components for skis and track
    LeftSkiComponent = CreateDefaultSubobject<USkiSceneComponent>(TEXT("LeftSkiComponent"));
    LeftSkiComponent->SetupAttachment(ChassisComponent);

    RightSkiComponent = CreateDefaultSubobject<USkiSceneComponent>(TEXT("RightSkiComponent"));
    RightSkiComponent->SetupAttachment(ChassisComponent);

    TrackComponent = CreateDefaultSubobject<UTrackSceneComponent>(TEXT("TrackComponent"));
    TrackComponent->SetupAttachment(ChassisComponent);

        // Left Ski mesh
    LeftSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSkiMesh"));
    LeftSkiMesh->SetupAttachment(LeftSkiComponent);

        // Right Ski mesh
    RightSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSkiMesh"));
    RightSkiMesh->SetupAttachment(RightSkiComponent);

        // Track mesh
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(TrackComponent);

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
    }

    if (RightSkiComponent)
    {
        RightSkiComponent->Chassis = ChassisComponent;
        RightSkiComponent->SkiMesh = RightSkiMesh;
    }

    if (TrackComponent)
    {
        TrackComponent->TrackMesh = TrackMesh;
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
        const FVector HorizontalVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f);

        if (!CurrentVelocity.IsNearlyZero())
        {
            const FVector BrakeDirection = -HorizontalVelocity.GetSafeNormal();
            const FVector BrakeForce = BrakeDirection * Value.Get<float>() * ChassisComponent->ForceMultiplier;

            Primitive->AddForce(BrakeForce, NAME_None, false);
        }
    }
}

void ASnowmobilePawn::Dismount(const FInputActionValue& Value)
{
    APolarisPlayerController* PlayerController = Cast<APolarisPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (Value.Get<float>() > 0.5f && PlayerController)
    {
        PlayerPawn->SetActorHiddenInGame(false);
        DetachPlayer(PlayerController);

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

void ASnowmobilePawn::AttachPlayer(APawn* InPlayerPawn, APolarisPlayerController* PlayerController)
{
    if(bIsPlayerAttached)
        return;

    bIsPlayerAttached = true;
    PlayerPawn = InPlayerPawn;

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
        }
    }

    // copy health from player pawn to snowmobile pawn
    if (PlayerPawn)
    {
        if (APolarisFPSCharacter* PlayerCharacter = Cast<APolarisFPSCharacter>(PlayerPawn))
        {
            Health = PlayerCharacter->Health;
            MaxHealth = PlayerCharacter->MaxHealth;
        }
    }

    if (ACharacter* Character = Cast<ACharacter>(PlayerPawn))
    {
        // hide player's mesh
        if (Character->GetMesh())
        {
            Character->GetMesh()->SetHiddenInGame(true);
        }

        // Disable movement and collision on player pawn
        if (UCharacterMovementComponent* CharMove = Character->GetCharacterMovement())
        {
            CharMove->DisableMovement();
        }
        if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
        {
            Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // attach player pawn to snowmobile root
        FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
        PlayerPawn->AttachToComponent(ChassisComponent, AttachRules, NAME_None);
    }

    // swap input to snowmobile   
        
    // disable input on player pawn
    if (PlayerPawn)
    {
        PlayerPawn->DisableInput(PlayerController);
    }

    // enable input on snowmobile pawn
    if (InputComponent == nullptr || !InputComponent->IsA<UEnhancedInputComponent>())
    {
        if (InputComponent)
        {
            InputComponent->DestroyComponent();
            InputComponent = nullptr;
        }

        InputComponent = NewObject<UEnhancedInputComponent>(this, TEXT("EnhancedInputComponent"));
        if (InputComponent)
        {
            InputComponent->RegisterComponent();
        }
    }
    EnableInput(PlayerController);
    SetupPlayerInputComponent(InputComponent);
    PlayerController->PushInputComponent(InputComponent);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        if (PlayerMappingContext == nullptr || SnowmobileMappingContext == nullptr)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("ChassisComponent::OnChassisHit - Input Mapping Contexts not set on SnowmobilePawn"));
            return;
        }

        Subsystem->RemoveMappingContext(PlayerMappingContext);
        Subsystem->AddMappingContext(SnowmobileMappingContext, 0);
    }
}

void ASnowmobilePawn::DetachPlayer(APolarisPlayerController* PlayerController)
{
    bIsPlayerAttached = false;

    // remove bindings
    if (UFoodWater* PlayerFoodWater = PlayerPawn->FindComponentByClass<UFoodWater>())
    {
        PlayerFoodWater->OnAppetite.RemoveDynamic(this, &ASnowmobilePawn::HandlePlayerAppetite);
        PlayerFoodWater->OnStarving.RemoveDynamic(this, &ASnowmobilePawn::HandlePlayerStarving);
    }

    // copy health back to player pawn and unhide
    if (APolarisFPSCharacter* PlayerCharacter = Cast<APolarisFPSCharacter>(PlayerPawn))
    {
        PlayerCharacter->Health = Health;
    }

    if (ACharacter* Character = Cast<ACharacter>(PlayerPawn))
    {
        // unhide the player's mesh
        if (Character->GetMesh())
        {
            Character->GetMesh()->SetHiddenInGame(false);
        }

        // enable movement and collision on player pawn
        if (UCharacterMovementComponent* CharMove = Character->GetCharacterMovement())
        {
            CharMove->SetMovementMode(MOVE_Walking);
        }
        if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
        {
            Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }

        // detach player pawn from snowmobile
        FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
        PlayerPawn->DetachFromActor(DetachRules);
    }

    // swap input back to player pawn
    DisableInput(PlayerController);
    PlayerPawn->EnableInput(PlayerController);
    PlayerController->PopInputComponent(InputComponent);
    // restore mapping contexts
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        if (PlayerMappingContext == nullptr || SnowmobileMappingContext == nullptr)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("SnowmobilePawn::DetachPlayer - Input Mapping Contexts not set on SnowmobilePawn"));
        }
        else
        {
            Subsystem->RemoveMappingContext(SnowmobileMappingContext);
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
        }
    }
}

void ASnowmobilePawn::HandlePlayerAppetite(float NewHunger, float NewThirst)
{
    if (!FoodWaterComponent)
        return;

    FoodWaterComponent->Hunger = NewHunger;
    FoodWaterComponent->Thirst = NewThirst;
}

void ASnowmobilePawn::HandlePlayerStarving(float Damage)
{
    if (!FoodWaterComponent)
        return;

    // forward starving event
    FoodWaterComponent->OnStarving.Broadcast(Damage);

    // apply damage to snowmobile pawn health
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

    // apply damage to player pawn health
    if (PlayerPawn)
    {
        if (APolarisFPSCharacter* PlayerCharacter = Cast<APolarisFPSCharacter>(PlayerPawn))
        {
            PlayerCharacter->Health = FMath::Clamp(PlayerCharacter->Health - Damage, 0.0f, PlayerCharacter->MaxHealth);
        }
    }
}