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
    LeftSkiMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftSkiMesh->SetCollisionProfileName(TEXT("NoCollision"));
    LeftSkiMesh->SetSimulatePhysics(false);

        // Right Ski mesh
    RightSkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSkiMesh"));
    RightSkiMesh->SetupAttachment(RightSkiComponent);
    RightSkiMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightSkiMesh->SetCollisionProfileName(TEXT("NoCollision"));
    RightSkiMesh->SetSimulatePhysics(false);

        // Track mesh
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(TrackComponent);
    TrackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TrackMesh->SetCollisionProfileName(TEXT("NoCollision"));
    TrackMesh->SetSimulatePhysics(false);

        // left ski Collision
    LeftSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSkiCollision"));
    LeftSkiCollision->SetupAttachment(LeftSkiComponent);
    LeftSkiCollision->SetBoxExtent(FVector(40.0f, 8.0f, 4.0f));
    LeftSkiCollision->SetRelativeLocation(FVector::ZeroVector);
    LeftSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    LeftSkiCollision->SetCollisionObjectType(ECC_WorldDynamic);
    LeftSkiCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    LeftSkiCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    LeftSkiCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    LeftSkiCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    LeftSkiCollision->SetNotifyRigidBodyCollision(true);
    LeftSkiCollision->SetGenerateOverlapEvents(false);
    LeftSkiCollision->SetSimulatePhysics(false);

        // right ski Collision
    RightSkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSkiCollision"));
    RightSkiCollision->SetupAttachment(RightSkiComponent);
    RightSkiCollision->SetBoxExtent(FVector(40.0f, 8.0f, 4.0f));
    RightSkiCollision->SetRelativeLocation(FVector::ZeroVector);
    RightSkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RightSkiCollision->SetCollisionObjectType(ECC_WorldDynamic);
    RightSkiCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    RightSkiCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    RightSkiCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    RightSkiCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    RightSkiCollision->SetNotifyRigidBodyCollision(true);
    RightSkiCollision->SetGenerateOverlapEvents(false);
    RightSkiCollision->SetSimulatePhysics(false);

        // track Collision
    TrackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrackCollision"));
    TrackCollision->SetupAttachment(TrackComponent);
    TrackCollision->SetBoxExtent(FVector(70.0f, 18.0f, 6.0f));
    TrackCollision->SetRelativeLocation(FVector::ZeroVector);
    TrackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TrackCollision->SetCollisionObjectType(ECC_WorldDynamic);
    TrackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    TrackCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    TrackCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    TrackCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    TrackCollision->SetNotifyRigidBodyCollision(true);
    TrackCollision->SetGenerateOverlapEvents(false);
    TrackCollision->SetSimulatePhysics(false);

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

void ASnowmobilePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update the attached player's controller rotation to match the seat orientation
    if (bIsPlayerAttached && AttachedPlayerController && ChassisComponent)
    {
        // calculate desired world rotation for the controller to match the seat rotation
        const FRotator DesiredWorldRotation = (ChassisComponent->GetComponentRotation() + SeatRelativeRotation + FRotator(0.0f, 180.0f, 0.0f)).GetNormalized();
        AttachedPlayerController->SetControlRotation(DesiredWorldRotation);
    }
}

void ASnowmobilePawn::Accelerate(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    const float InputValue = Value.Get<float>();

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector Forward = -ChassisComponent->GetForwardVector();
        const FVector Force = Forward * InputValue * ChassisComponent->ForceMultiplier;
        Primitive->AddForce(Force, NAME_None, false);
    }
}

void ASnowmobilePawn::Steer(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        float Input = Value.Get<float>();

        const FVector Velocity = Primitive->GetPhysicsLinearVelocity();
        const float ForwardSpeed = FVector::DotProduct(Velocity, GetActorForwardVector());
        const float SpeedScale = FMath::Clamp(FMath::Abs(ForwardSpeed) / 600.0f, 0.0f, 1.0f);

        const FVector Torque = ChassisComponent->GetUpVector() * Input * ChassisComponent->TorqueMultiplier * SpeedScale;

        Primitive->AddTorqueInRadians(Torque, NAME_None, false);
    }
}

void ASnowmobilePawn::Brake(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector CurrentVelocity = Primitive->GetPhysicsLinearVelocity();
        const FVector Forward = GetActorForwardVector();

        const float ForwardSpeed = FVector::DotProduct(CurrentVelocity, Forward);
        if (FMath::IsNearlyZero(ForwardSpeed))
            return;

        const float Strength = FMath::Abs(Value.Get<float>()) * ChassisComponent->ForceMultiplier;
        const float DeltaTime = GetWorld()->GetDeltaSeconds();

        const float SpeedReduction = Strength * DeltaTime;
        float NewForwardSpeed;
        if (ForwardSpeed > 0.0f)
            NewForwardSpeed = FMath::Max(0.0f, ForwardSpeed - SpeedReduction);
        else
            NewForwardSpeed = FMath::Min(0.0f, ForwardSpeed + SpeedReduction);

        const FVector VelocityDelta = (NewForwardSpeed - ForwardSpeed) * Forward;
        Primitive->SetPhysicsLinearVelocity(CurrentVelocity + VelocityDelta);
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
        // Determine facing direction brfore disabling movement for attachment
        FVector FacingDirection = FVector::ZeroVector;
        float Speed = 0.0f;
        UCharacterMovementComponent* CharacterMoveComponent = Character->GetCharacterMovement();
        if (CharacterMoveComponent)
        {
            Speed = CharacterMoveComponent->Velocity.Size();
            FacingDirection = CharacterMoveComponent->Velocity.GetSafeNormal();
        }
        else
        {
            Speed = Character->GetVelocity().Size();
            FacingDirection = Character->GetVelocity().GetSafeNormal();
        }

        // If almost stationary, use mesh or actor forward
        const float SpeedThreshold = 10.0f;
        if (Speed <= SpeedThreshold)
        {
            if (Character->GetMesh())
                FacingDirection = Character->GetMesh()->GetForwardVector();
            else
                FacingDirection = Character->GetActorForwardVector();
        }

        // hide player's mesh
        if (Character->GetMesh())
        {
            Character->GetMesh()->SetHiddenInGame(true);
        }

        // Disable movement and collision on player pawn
        if (CharacterMoveComponent)
        {
            CharacterMoveComponent->DisableMovement();
        }
        if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
        {
            Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // flip the seat yaw based on facing or chassis forward
        const FVector ChassisForward = ChassisComponent->GetForwardVector().GetSafeNormal();
        FVector UseFacing = FacingDirection.GetSafeNormal();
        // if UseFacing is degenerate, fall back to actor forward
        if (UseFacing.IsNearlyZero())
            UseFacing = Character->GetActorForwardVector().GetSafeNormal();

        // final seat rotation
        FRotator FinalSeatRotation = SeatRelativeRotation;
        FinalSeatRotation.Yaw += 180.0f;
        FinalSeatRotation = FinalSeatRotation.GetNormalized();

        // attach player pawn to chassis
        FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
        PlayerPawn->AttachToComponent(ChassisComponent, AttachRules, NAME_None);

        // position and rotate relative to chassis
        PlayerPawn->SetActorRelativeLocation(SeatRelativeLocation);
        PlayerPawn->SetActorRelativeRotation(FinalSeatRotation);

        // set controller rotation to match seat in world space
        const FRotator DesiredWorldRotation = (ChassisComponent->GetComponentRotation() + FinalSeatRotation).GetNormalized();
        PlayerController->SetControlRotation(DesiredWorldRotation);

        // make player pawn face controller and not rotate with movement
        Character->bUseControllerRotationYaw = true;
        if (UCharacterMovementComponent* CharacterMove = Character->GetCharacterMovement())
            CharacterMove->bOrientRotationToMovement = false;

        // set the player controller's pawn
        AttachedPlayerController = PlayerController;
    }

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

    // clear attached controller pointer
    AttachedPlayerController = nullptr;

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