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
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
    LeftSkiCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
    LeftSkiCollision->SetNotifyRigidBodyCollision(true);
    LeftSkiCollision->SetGenerateOverlapEvents(true);
    LeftSkiCollision->SetSimulatePhysics(true);

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
    RightSkiCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
    RightSkiCollision->SetNotifyRigidBodyCollision(true);
    RightSkiCollision->SetGenerateOverlapEvents(true);
    RightSkiCollision->SetSimulatePhysics(true);

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
    TrackCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
    TrackCollision->SetNotifyRigidBodyCollision(true);
    TrackCollision->SetGenerateOverlapEvents(true);
    TrackCollision->SetSimulatePhysics(true);

        // Create physics constraints
    LeftSkiConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftSkiConstraint"));
    LeftSkiConstraint->SetupAttachment(ChassisComponent);

    RightSkiConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightSkiConstraint"));
    RightSkiConstraint->SetupAttachment(ChassisComponent);

    TrackConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TrackConstraint"));
    TrackConstraint->SetupAttachment(ChassisComponent);

        // Food and Water component
    FoodWaterComponent = CreateDefaultSubobject<UFoodWater>(TEXT("FoodWaterComponent"));

        // Camera boom and camera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(ChassisComponent);
    CameraBoom->TargetArmLength = 350.0f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = true;
    CameraBoom->bInheritYaw = true;
    CameraBoom->bInheritRoll = true;
    CameraBoom->SetRelativeLocation(SeatRelativeLocation);
    CameraBoom->SetRelativeRotation(SeatRelativeRotation + FRotator(-10.0f, 0.0f, 0.0f));

    SnowmobileCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SnowmobileCamera"));
    SnowmobileCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    SnowmobileCamera->bUsePawnControlRotation = false;
    SnowmobileCamera->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void ASnowmobilePawn::BeginPlay()
{
    Super::BeginPlay();

    // Set up component references
    if (ChassisComponent)
    {
        ChassisComponent->SnowmobilePawn = this;
    }
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

    // Attach constraints to physics bodies
    UPrimitiveComponent* ChassisPrimitive = Cast<UPrimitiveComponent>(ChassisComponent);

    // Stabilization
    if (ChassisPrimitive)
    {
        // Lower center of mass slightly to increase stability
        ChassisPrimitive->SetCenterOfMass(FVector(0.0f, 0.0f, -25.0f));

        // Add angular damping so the snowmobile resists unwanted rotation
        ChassisPrimitive->SetAngularDamping(BaseAngularDamping);

        // Small linear damping to avoid jittery acceleration
        ChassisPrimitive->SetLinearDamping(0.4f);
    }

    if (LeftSkiConstraint && ChassisPrimitive && LeftSkiCollision)
    {
        LeftSkiConstraint->SetConstrainedComponents(ChassisPrimitive, NAME_None, LeftSkiCollision, NAME_None);

        // lock linear motion
        LeftSkiConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        LeftSkiConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        LeftSkiConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

        // allow small angular compliance for skis
        LeftSkiConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 1.0f);
        LeftSkiConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 1.0f);
        LeftSkiConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 1.0f);

        LeftSkiConstraint->SetDisableCollision(true);
    }

    if (RightSkiConstraint && ChassisPrimitive && RightSkiCollision)
    {
        RightSkiConstraint->SetConstrainedComponents(ChassisPrimitive, NAME_None, RightSkiCollision, NAME_None);

        // lock linear motion
        RightSkiConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        RightSkiConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        RightSkiConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

        // allow small angular compliance for skis
        RightSkiConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 1.0f);
        RightSkiConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 1.0f);
        RightSkiConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 1.0f);

        RightSkiConstraint->SetDisableCollision(true);
    }

    if (TrackConstraint && ChassisPrimitive && TrackCollision)
    {
        TrackConstraint->SetConstrainedComponents(ChassisPrimitive, NAME_None, TrackCollision, NAME_None);

        // lock linear motion for track
        TrackConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        TrackConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
        TrackConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

        // track have less angular freedom
        TrackConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 0.5f);
        TrackConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 0.5f);
        TrackConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 0.5f);

        TrackConstraint->SetDisableCollision(true);
    }

    // Set up collision bindings
    if (LeftSkiCollision && LeftSkiComponent)
    {
        LeftSkiCollision->OnComponentBeginOverlap.AddDynamic(LeftSkiComponent, &USkiSceneComponent::OnSkiBeginOverlap);
        LeftSkiCollision->OnComponentHit.AddDynamic(LeftSkiComponent, &USkiSceneComponent::OnSkiHit);
    }
    if (RightSkiCollision && RightSkiComponent)
    {
        RightSkiCollision->OnComponentBeginOverlap.AddDynamic(RightSkiComponent, &USkiSceneComponent::OnSkiBeginOverlap);
        RightSkiCollision->OnComponentHit.AddDynamic(RightSkiComponent, &USkiSceneComponent::OnSkiHit);
    }
    if (TrackCollision && TrackComponent)
    {
        TrackCollision->OnComponentBeginOverlap.AddDynamic(TrackComponent, &UTrackSceneComponent::OnTrackBeginOverlap);
        TrackCollision->OnComponentHit.AddDynamic(TrackComponent, &UTrackSceneComponent::OnTrackHit);
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

    // stabilization
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector AngleVelocity = Primitive->GetPhysicsAngularVelocityInRadians();
        const float YawRate = AngleVelocity.Z;

        // deadzone so tiny numerical yaw doesn't produce impulses
        const float Deadzone = 0.02f;
        if (FMath::Abs(YawRate) > Deadzone && StabilizationGain > 0.0f)
        {
            float TorqueScalar = -YawRate * StabilizationGain * DeltaTime;

            // Clamp to a safe range
            TorqueScalar = FMath::Clamp(TorqueScalar, -MaxStabilizationTorque, MaxStabilizationTorque);
            const FVector Torque = FVector::UpVector * TorqueScalar;
            Primitive->AddTorqueInRadians(Torque, NAME_None, false);
        }
    }
    
    // Aerodynamic drag
    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const FVector Velocity = Primitive->GetPhysicsLinearVelocity();
        const float Speed = Velocity.Size();
        if (Speed > KINDA_SMALL_NUMBER)
        {
            const float AirDragCoefficient = 0.0008f; // higher = stronger drag
            const float FrontalArea = 1.8f;
            const float Density = 1.225f;

            // Drag converted into a force
            const float DragMagnitude = 0.5f * Density * AirDragCoefficient * FrontalArea * Speed * Speed;

            // scale by mass so effect is consistent
            const FVector DragForce = -Velocity.GetSafeNormal() * DragMagnitude * Primitive->GetMass(); 

            Primitive->AddForce(DragForce, NAME_None, false);
        }
    }

    // Update the attached player's controller rotation to match the seat orientation
    if (bIsPlayerAttached && AttachedPlayerController && ChassisComponent)
    {
        // calculate desired world rotation for the controller to match the seat rotation
        const FRotator DesiredWorldRotation = 
            (ChassisComponent->GetComponentRotation() + SeatRelativeRotation + FRotator(0.0f, 180.0f, 0.0f)).GetNormalized();
        AttachedPlayerController->SetControlRotation(DesiredWorldRotation);

        // Smoothly make the camera lean with the chassis and follow yaw/roll
        if (CameraBoom)
        {
            // Target relative rotation
            FRotator TargetRelative = (SeatRelativeRotation + FRotator(-10.0f, 180.0f, 0.0f)).GetNormalized();

            // zero relative roll so chassis roll passes through
            TargetRelative.Roll = 0.0f;

            const FRotator CurrentRelative = CameraBoom->GetRelativeRotation();

            // Interpolate the relative rotation to keep it smooth
            const float CameraInterpolateSpeed = 6.0f;
            const FRotator NewRelative = FMath::RInterpTo(CurrentRelative, TargetRelative, DeltaTime, CameraInterpolateSpeed);

            CameraBoom->SetRelativeRotation(NewRelative);
        }
    }
}

void ASnowmobilePawn::Accelerate(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent || !TrackCollision)
        return;

    const float InputValue = FMath::Clamp(Value.Get<float>(), -1.0f, 1.0f);
    UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent);
    if (!Primitive)
        return;

    // downward trace from the track to detect contact and slope
    FHitResult Hit;
    const FVector Start = TrackCollision->GetComponentLocation();
    const FVector End = Start - FVector(0.0f, 0.0f, 220.0f);
    const FCollisionQueryParams Params(SCENE_QUERY_STAT(TrackContact), true, this);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    // base traction when contacting ground
    float Traction = 0.0f;
    if (bHit)
    {
        Traction = 1.0f;
    }

    // reduce traction on steep slopes
    if (bHit)
    {
        // compute slope angle from surface normal
        const float NormalDotUp = FMath::Clamp(FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector), -1.0f, 1.0f);
        const float SlopeAngleDeg = FMath::RadiansToDegrees(FMath::Acos(NormalDotUp)); // 0 == flat, 90 == vertical

        // max climb angle and penalty for pushing uphill
        const float MaxClimbAngleDegrees = 35.0f;
        const float UphillPenalty = 1.6f;

        // if slope exceeds max climb angle, no traction
        if (SlopeAngleDeg > MaxClimbAngleDegrees)
        {
            Traction = 0.0f;
        }
        else
        {
            // gentle reduction as slope increases (flat == 1.0, near MaxClimbAngleDegrees == 0.4)
            const float 
                FlatReference = 0.0f;
            const float 
                SlopeLerpT = FMath::Clamp((SlopeAngleDeg - FlatReference) / (MaxClimbAngleDegrees - FlatReference), 0.0f, 1.0f);
            const float 
                SlopeTractionScale = FMath::Lerp(1.0f, 0.4f, SlopeLerpT);
            Traction *= SlopeTractionScale;

            // determine if pushing uphill
            const FVector ForwardDirection = -ChassisComponent->GetForwardVector().GetSafeNormal();
            const float UphillFactor = FMath::Clamp(FVector::DotProduct(ForwardDirection, -Hit.ImpactNormal), 0.0f, 1.0f);

            // scale traction further when pushing uphill
            const float UphillScale = FMath::Clamp(1.0f - (UphillFactor * UphillPenalty * SlopeLerpT), 0.0f, 1.0f);
            Traction *= UphillScale;
        }
    }

    // track power curve
    const float PowerScale = FMath::Pow(FMath::Abs(InputValue), 0.9f);
    const float ForwardSign = FMath::Sign(InputValue);

    // forward direction is along the track/chassis forward but tracks push slightly downward/backward
    const FVector ForwardDirection = -ChassisComponent->GetForwardVector().GetSafeNormal();

    // Compute force and apply at the track location so the chassis pitches naturally
    const float BaseForce = ChassisComponent->ForceMultiplier * AccelerationMultiplier;

    // Current speed along the forward direction
    const float CurrentForwardSpeed = FVector::DotProduct(Primitive->GetPhysicsLinearVelocity(), ForwardDirection);

    // Desired speed in the forward direction
    const float DesiredDirectionSpeed = ForwardSign * CurrentForwardSpeed;

    // if no traction, apply small opposing force to avoid teleporting up slopes
    if (Traction <= KINDA_SMALL_NUMBER)
    {
        const FVector SmallResist = -ForwardDirection * 10.0f * Primitive->GetMass();
        Primitive->AddForceAtLocation(SmallResist, TrackCollision->GetComponentLocation(), NAME_None);
    }
    else
    {
        if (DesiredDirectionSpeed >= MaxForwardSpeed && MaxForwardSpeed > 0.0f)
        {
            // apply drag
            const float DragForceMagnitude = ChassisComponent->ForceMultiplier * 0.5f * AccelerationMultiplier;
            const FVector DragForce = -ForwardDirection * DragForceMagnitude * ForwardSign * PowerScale * Traction;
            Primitive->AddForceAtLocation(DragForce, TrackCollision->GetComponentLocation(), NAME_None);
        }
        else
        {
            // Taper force as speed approaches max so acceleration feels smooth.
            float SpeedRatio = 0.0f;
            if(MaxForwardSpeed > 0.0f)
            {
                SpeedRatio = FMath::Clamp(DesiredDirectionSpeed / MaxForwardSpeed, 0.0f, 1.0f);
            }
            
            const float ForceSpeedScale = FMath::Lerp(1.0f, 0.05f, SpeedRatio);

            const FVector Force = ForwardDirection * ForwardSign * BaseForce * PowerScale * Traction * ForceSpeedScale;
            Primitive->AddForceAtLocation(Force, TrackCollision->GetComponentLocation(), NAME_None);
        }
    }
}

void ASnowmobilePawn::Steer(const FInputActionValue& Value)
{
    if (!ChassisComponent)
        return;

    const float RawInput = Value.Get<float>();
    const float Input = FMath::Clamp(RawInput, -1.0f, 1.0f);

    UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent);
    if (!Primitive)
        return;

    // check if input is active
    const bool bInputActive = FMath::Abs(Input) > KINDA_SMALL_NUMBER;
    if (bInputActive)
    {
        if (!bIsSteeringInputActive)
        {
            // steering just began | save existing damping
            if (SavedAngularDamping < 0.0f)
                SavedAngularDamping = BaseAngularDamping;

            // temporarily reduce damping for steering torque
            Primitive->SetAngularDamping(SteeringAngularDamping);

            bIsSteeringInputActive = true;
        }

        // continuous torque while steering
        const FVector Velocity = Primitive->GetPhysicsLinearVelocity();
        const float ForwardSpeed = FVector::DotProduct(Velocity, GetActorForwardVector());

        // strong steering at low speeds, limited at high speeds
        const float LowSpeedFull = 200.0f; // speeds below this get full steering
        const float HighSpeedFade = 1000.0f; // above this steering is reduced
        const float SpeedAbs = FMath::Abs(ForwardSpeed);
        float SpeedScale = 1.0f;
        if (SpeedAbs > LowSpeedFull)
        {
            SpeedScale = FMath::Clamp(1.0f - ((SpeedAbs - LowSpeedFull) / (HighSpeedFade - LowSpeedFull)), 0.25f, 1.0f);
        }

        // Scale torque
        const FVector Torque = ChassisComponent->GetUpVector() * Input * ChassisComponent->TorqueMultiplier * SteeringGain * SteeringTorqueMultiplier * SpeedScale;
        Primitive->AddTorqueInRadians(Torque, NAME_None, false);
    }
    else
    {
        // restore angular damping if modified
        if (bIsSteeringInputActive)
        {
            // restore base damping
            if(SavedAngularDamping > 0.0f)
            {
                Primitive->SetAngularDamping(SavedAngularDamping);
            }
            else
            {
                Primitive->SetAngularDamping(BaseAngularDamping);
            }
            
            bIsSteeringInputActive = false;
        }
    }
}

void ASnowmobilePawn::Brake(const FInputActionValue& Value)
{
    if (FMath::Abs(Value.Get<float>()) <= KINDA_SMALL_NUMBER || !ChassisComponent)
        return;

    if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ChassisComponent))
    {
        const float BrakeInput = FMath::Clamp(FMath::Abs(Value.Get<float>()), 0.0f, 1.0f);
        if (BrakeInput <= KINDA_SMALL_NUMBER)
        {
            // Restore base damping if no brake input
            Primitive->SetLinearDamping(0.4f);
            return;
        }

        const FVector CurrentVelocity = Primitive->GetPhysicsLinearVelocity();
        const FVector Forward = GetActorForwardVector().GetSafeNormal();
        const float ForwardSpeed = FVector::DotProduct(CurrentVelocity, Forward);

        // If stopped along forward axis, clear only the forward and avoid applying forces
        const float NearStopThreshold = 10.0f;
        if (FMath::Abs(ForwardSpeed) <= NearStopThreshold)
        {
            // prevent jitter
            const FVector NewVelocity = CurrentVelocity - Forward * ForwardSpeed;
            Primitive->SetPhysicsLinearVelocity(NewVelocity, false, NAME_None);

            // Ensure damping is reasonable so small residuals die out smoothly
            Primitive->SetLinearDamping(0.4f);
            return;
        }

        // compute deceleration and apply as a normal force
        const float BrakeDeceleration = 1200.0f * BrakeInput;
        const float DeltaTime = FMath::Max(GetWorld()->GetDeltaSeconds(), 1e-6f);
        const float MaxStoppingDecel = FMath::Abs(ForwardSpeed) / DeltaTime;
        const float AppliedDecel = FMath::Min(BrakeDeceleration, MaxStoppingDecel);

        const float Mass = Primitive->GetMass();
        const FVector BrakeForce = -Forward * AppliedDecel * Mass * FMath::Sign(ForwardSpeed);

        // Apply force normally so physics integrates naturally
        Primitive->AddForce(BrakeForce, NAME_None, false);

        // Smoothly increase linear damping while braking to absorb small residual motion
        const float BaseDamping = 0.4f;
        const float BrakeDamping = 3.0f;
        const float NewDamping = FMath::Lerp(BaseDamping, BrakeDamping, BrakeInput);
        Primitive->SetLinearDamping(NewDamping);
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

        // Save rotation state
        SavedUseControllerRotationYaw = Character->bUseControllerRotationYaw;
        if (CharacterMoveComponent)
            SavedOrientRotationToMovement = CharacterMoveComponent->bOrientRotationToMovement;
        if (PlayerController)
            SavedControllerRotation = PlayerController->GetControlRotation();
        if (Character->GetMesh())
        {
            SavedMeshRelativeRotation = Character->GetMesh()->GetRelativeRotation();
            SavedActorRotation = Character->GetActorRotation();
            bHasSavedRotations = true;
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

    // Switch camera
    if (PlayerController)
    {
        PreviousViewTarget = PlayerController->GetViewTarget();
        PlayerController->SetViewTargetWithBlend(this, 0.0f);
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

        // Restore rotation state
        Character->bUseControllerRotationYaw = SavedUseControllerRotationYaw;
        if (UCharacterMovementComponent* CharacterMove = Character->GetCharacterMovement())
        {
            CharacterMove->bOrientRotationToMovement = SavedOrientRotationToMovement;
        }
        if (bHasSavedRotations)
        {
            // restore actor rotation
            Character->SetActorRotation(SavedActorRotation);

            // restore mesh relative rotation
            if (Character->GetMesh())
            {
                Character->GetMesh()->SetRelativeRotation(SavedMeshRelativeRotation);
            }

            bHasSavedRotations = false;
        }

        // restore controller rotation
        if (PlayerController)
        {
            // Restore the controller rotation
            PlayerController->SetControlRotation(SavedControllerRotation);
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

    // restore camera
    if (PlayerController)
    {
        if (PreviousViewTarget)
        {
            PlayerController->SetViewTargetWithBlend(PreviousViewTarget, 0.2f);
            PreviousViewTarget = nullptr;
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